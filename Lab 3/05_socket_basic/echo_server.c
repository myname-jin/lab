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
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;

    // 1. 서버 소켓 생성 (IPv4, TCP 스트림)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) error_handling("socket() error");

    // 2. 주소 구조체 초기화 및 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;                // IPv4 주소 체계
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 현재 시스템의 IP 주소 자동 할당
    serv_addr.sin_port = htons(PORT);              // 포트 번호 할당

    // 3. 소켓에 IP 주소와 포트 번호 할당 (Binding)
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // 4. 연결 요청 대기 상태로 진입 (Listening, 대기열 크기: 5)
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    printf("Server Start! Waiting for connection...\n");

    clnt_addr_size = sizeof(clnt_addr);

    // 5. 클라이언트의 연결 요청 수락 (Blocking 모드)
    // 연결 요청이 들어올 때까지 대기하며, 연결 시 새로운 소켓 디스크립터 반환
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) error_handling("accept() error");
    
    printf("Client Connected!\n");

    // 6. 데이터 송수신 (Echo 서비스 수행)
    // 클라이언트로부터 수신된 데이터를 버퍼에 저장 후 즉시 재전송
    while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
        write(clnt_sock, message, str_len); 
    }

    // 7. 소켓 리소스 해제 및 종료
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
