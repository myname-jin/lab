#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define PORT 9190
#define IP "127.0.0.1"

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len, recv_len, recv_cnt;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        printf("Connected to Server! (Type 'Q' to quit)\n");

    while (1) {
        fputs("Input message: ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        str_len = write(sock, message, strlen(message)); // 1. 데이터 전송 및 길이 저장
        
        recv_len = 0;
        // 2. [핵심 수정] 보낸 길이(str_len)만큼 다 받을 때까지 반복해서 읽음
        while (recv_len < str_len) {
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1) error_handling("read() error");
            recv_len += recv_cnt; // 읽은 만큼 누적
        }
        
        message[recv_len] = 0; // 문자열 끝 처리
        printf("Message from server: %s", message);
    }

    close(sock);
    return 0;
}
