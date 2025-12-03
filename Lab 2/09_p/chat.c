#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // read, write, close, STDIN_FILENO
#include <string.h>     // strlen, bzero, strcmp
#include <fcntl.h>      // O_RDWR, O_WRONLY, O_NONBLOCK
#include <sys/select.h> // select(), FD_SET, FD_ZERO, FD_ISSET
#include <sys/types.h>  // mkfifo()
#include <sys/stat.h>   // mkfifo()

#define BUF_SIZE 256

const char *FIFO_A = "chat_fifo1"; // 1 -> 2
const char *FIFO_B = "chat_fifo2"; // 2 -> 1

int main(int argc, char *argv[])
{
    int read_fd, write_fd;
    char buf[BUF_SIZE];
    int nbytes;
    fd_set read_fds;
    int maxfd;

    if (argc != 2) {
        fprintf(stderr, "사용법: ./fifo_chat [사용자번호 (1 또는 2)]\n");
        exit(1);
    }

    // FIFO 파일 2개 생성 (이미 있으면 무시됨)
    mkfifo(FIFO_A, 0666);
    mkfifo(FIFO_B, 0666);

    // 사용자 번호에 따라 읽기/쓰기 파이프 설정
    if (strcmp(argv[1], "1") == 0) {
        printf("--- 채팅 클라이언트 1 (송신: %s, 수신: %s) ---\n", FIFO_A, FIFO_B);
        printf("상대방이 접속할 때까지 잠시 기다릴 수 있습니다...\n");
        
        // 1번 사용자는 A(1)에 쓰고, B(2)에서 읽음
        write_fd = open(FIFO_A, O_WRONLY); 
        read_fd = open(FIFO_B, O_RDONLY); // 상대가 open할 때까지 대기(Block)

    } else if (strcmp(argv[1], "2") == 0) {
        printf("--- 채팅 클라이언트 2 (송신: %s, 수신: %s) ---\n", FIFO_B, FIFO_A);
        
        // [수정된 부분] 2번 사용자는 A(1)에서 먼저 읽고, B(2)에 씀
        read_fd = open(FIFO_A, O_RDONLY);
        write_fd = open(FIFO_B, O_WRONLY);
    } else {
        fprintf(stderr, "사용자 번호는 1 또는 2 여야 합니다.\n");
        exit(1);
    }

    if (read_fd < 0 || write_fd < 0) {
        perror("FIFO 열기 실패");
        exit(1);
    }
    
    printf("--- 채팅이 연결되었습니다. 'quit' 입력 시 종료 ---\n");

    // select()가 감시할 파일 디스크립터 최대값 설정
    maxfd = (read_fd > STDIN_FILENO ? read_fd : STDIN_FILENO) + 1;

    while (1) {
        // 감시 대상 초기화
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds); // 키보드(0번) 감시
        FD_SET(read_fd, &read_fds);      // 상대방 파이프 감시

        printf("나: ");
        fflush(stdout);

        // 키보드 또는 상대방 파이프에서 입력이 올 때까지 대기
        if (select(maxfd, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select error");
            break;
        }

        // 키보드 입력이 감지된 경우 (내가 메시지 보냄)
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            nbytes = read(STDIN_FILENO, buf, BUF_SIZE);
            if (nbytes > 0) {
                buf[nbytes] = '\0';
                write(write_fd, buf, nbytes); // 상대방 파이프로 전송

                if (strncmp(buf, "quit", 4) == 0) {
                    break;
                }
            }
        }

        // 상대방 파이프 입력이 감지된 경우 (상대 메시지 도착)
        if (FD_ISSET(read_fd, &read_fds)) {
            nbytes = read(read_fd, buf, BUF_SIZE);
            
            if (nbytes <= 0) {
                printf("\r상대방이 채팅을 종료했습니다.\n");
                break;
            }

            buf[nbytes] = '\0';
            // "\r"은 "나: " 프롬프트를 덮어쓰고 줄의 맨 앞으로 이동
            printf("\r상대방: %s", buf); 

            if (strncmp(buf, "quit", 4) == 0) {
                break;
            }
        }
    }

    close(read_fd);
    close(write_fd);
    printf("--- 채팅을 종료합니다. ---\n");
    return 0;
}
