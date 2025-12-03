#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define PORT 9190
#define IP "127.0.0.1"

void error_handling(char *buf) { perror(buf); exit(1); }

int main(int argc, char *argv[]) {
    int sock, str_len;
    char buf[BUF_SIZE], name[BUF_SIZE], msg[BUF_SIZE * 2];
    struct sockaddr_in serv_adr;
    fd_set reads, cpy_reads;
    
    // 실행 시 이름 입력 (예: ./chat_client Jinho)
    if (argc != 2) {
        printf("Usage : %s <Name>\n", argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[1]);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(IP);
    serv_adr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    
    printf("Chat Connected!\n");

    FD_ZERO(&reads);
    FD_SET(0, &reads);    // 0: 표준 입력(키보드) 감시
    FD_SET(sock, &reads); // sock: 서버 소켓 감시

    while (1) {
        cpy_reads = reads;
        if (select(sock + 1, &cpy_reads, 0, 0, 0) == -1) break;

        // 1. 서버에서 데이터 옴 (수신)
        if (FD_ISSET(sock, &cpy_reads)) {
            str_len = read(sock, buf, BUF_SIZE - 1);
            if (str_len == 0) return 0;
            buf[str_len] = 0;
            printf("%s", buf); // 화면 출력
        }

        // 2. 키보드 입력 있음 (송신)
        if (FD_ISSET(0, &cpy_reads)) {
            str_len = read(0, buf, BUF_SIZE); 
            if (str_len == 0) break;
            buf[str_len] = 0;
            
            sprintf(msg, "%s %s", name, buf); // 이름 붙여서 전송
            write(sock, msg, strlen(msg));
        }
    }
    close(sock);
    return 0;
}
