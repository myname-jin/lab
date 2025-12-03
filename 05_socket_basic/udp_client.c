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
    struct sockaddr_in serv_addr, from_addr;
    socklen_t adr_sz;
    char message[BUF_SIZE];
    int str_len;

    // 1. UDP 소켓 생성
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) error_handling("socket() error");

    // 보낼 곳(서버)의 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP);
    serv_addr.sin_port = htons(PORT);

    while (1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 2. 데이터 전송 (sendto)
        // 목적지 주소(serv_addr)를 인자로 직접 지정합니다.
        sendto(sock, message, strlen(message), 0, 
               (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        adr_sz = sizeof(from_addr);
        
        // 3. 데이터 수신 (recvfrom)
        // 서버가 에코로 돌려보낸 데이터를 받습니다.
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
                           (struct sockaddr*)&from_addr, &adr_sz);
        
        message[str_len] = 0;
        printf("Message from server: %s", message);
    }

    close(sock);
    return 0;
}
