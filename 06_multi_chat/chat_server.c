#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define PORT 9190

void error_handling(char *buf) { perror(buf); exit(1); }

int main() {
    int serv_sock, clnt_sock, fd_max, str_len, i, j;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    char buf[BUF_SIZE];
    fd_set reads, cpy_reads;

    // 1. 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) error_handling("bind() error");
    if (listen(serv_sock, 5) == -1) error_handling("listen() error");

    // 2. select 초기화
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads); // 서버 소켓 등록 (연결 요청 감시)
    fd_max = serv_sock;

    printf("Chat Server Start!\n");

    while (1) {
        cpy_reads = reads;
        // 3. 변화 감지 (무한 대기)
        if (select(fd_max + 1, &cpy_reads, 0, 0, 0) == -1) break;

        for (i = 0; i < fd_max + 1; i++) {
            if (FD_ISSET(i, &cpy_reads)) { // 변화가 있는 소켓 발견
                
                // Case A: 연결 요청 (서버 소켓)
                if (i == serv_sock) {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads); // 감시 목록에 추가
                    if (fd_max < clnt_sock) fd_max = clnt_sock;
                    printf("Connected client: %d\n", clnt_sock);
                }
                // Case B: 데이터 수신 (클라이언트 소켓)
                else {
                    str_len = read(i, buf, BUF_SIZE);
                    if (str_len == 0) { // 연결 종료
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d\n", i);
                    } else { 
                        // 메시지 방송 (Broadcast): 나(i)를 제외한 모든 접속자에게 전송
                        for (j = 0; j < fd_max + 1; j++) {
                            if (FD_ISSET(j, &reads) && j != serv_sock && j != i)
                                write(j, buf, str_len);
                        }
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
