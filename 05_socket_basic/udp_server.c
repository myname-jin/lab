#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define PORT 9190

void error_handling(char *message) {
    perror(message);
    exit(1);
}

int main() {
    int serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;

    // 1. UDP 소켓 생성 (SOCK_DGRAM)
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1) error_handling("UDP socket creation error");

    // 2. 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // 3. 소켓에 주소 할당 (Bind)
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    printf("UDP Server Start!\n");

    while (1) {
        clnt_addr_size = sizeof(clnt_addr);
        
        // 4. 데이터 수신 (recvfrom)
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, 
                           (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        
        // 5. 데이터 송신 (sendto)
      
        sendto(serv_sock, message, str_len, 0, 
               (struct sockaddr*)&clnt_addr, clnt_addr_size);
    }

    close(serv_sock);
    return 0;
}
