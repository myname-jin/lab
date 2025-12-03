#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h> 

#define BUF_SIZE 1024
#define PORT 8080

void error_handling(char *buf) { perror(buf); exit(1); }

// 정적 파일(HTML) 전송 함수 (GET 요청)
void send_file(int clnt_sock, char *file_name) {
    char buf[BUF_SIZE];
    FILE *fp;
    
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        write(clnt_sock, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
        return;
    }

    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
    write(clnt_sock, header, strlen(header));

    while (fgets(buf, BUF_SIZE, fp) != NULL) {
        write(clnt_sock, buf, strlen(buf));
    }
    fclose(fp);
}

// CGI 실행 함수 (POST 요청) - 파이프 사용
// read_data: 부모가 소켓에서 이미 읽은 데이터 (POST Body)
void execute_cgi(int clnt_sock, char *path, char *method, char *read_data) {
    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
    write(clnt_sock, header, strlen(header));

    int fds[2]; // 파이프 생성 (0: 읽기용, 1: 쓰기용)
    if (pipe(fds) == -1) {
        perror("pipe error");
        return;
    }

    int pid = fork();

    if (pid == 0) { // [자식 프로세스: 파이썬 실행]
        close(fds[1]); // 자식은 쓰기 안 함
        
        // 1. 파이프의 읽기 구멍(fds[0])을 표준 입력(STDIN)으로 연결
        // -> 부모가 파이프에 넣어준 데이터를 sys.stdin.read()로 읽을 수 있게 됨
        dup2(fds[0], STDIN_FILENO); 
        
        // 2. 소켓을 표준 출력(STDOUT)으로 연결
        // -> print() 하면 클라이언트에게 날아감
        dup2(clnt_sock, STDOUT_FILENO); 
        
        // 3. 파이썬 실행
        execl("/usr/bin/python3", "python3", path, NULL);
        exit(0);
    } else { // [부모 프로세스: 데이터 전달]
        close(fds[0]); // 부모는 읽기 안 함
        
        // 4. 아까 읽어둔 데이터(read_data)를 파이프에 밀어넣음
        write(fds[1], read_data, strlen(read_data));
        
        // 5. 다 썼으니 파이프 닫음 (자식에게 EOF 전달 -> 자식의 read가 끝남)
        close(fds[1]); 
        
        // 부모 역할 끝. 소켓은 자식이 쓰고 있으니 부모 쪽에서는 닫음
        close(clnt_sock); 
        
        // 자식 프로세스 정리 (좀비 방지)
        waitpid(pid, NULL, WNOHANG); 
    }
}

int main() {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    char buf[BUF_SIZE];
    char method[10], path[100];

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(PORT);

    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) error_handling("bind() error");
    if (listen(serv_sock, 5) == -1) error_handling("listen() error");

    printf("Web Server Started on Port %d...\n", PORT);

    while (1) {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if (clnt_sock == -1) continue;

        // 1. 요청 읽기 (여기서 소켓 데이터를 읽어 buf에 저장)
        int len = read(clnt_sock, buf, BUF_SIZE - 1);
        if (len > 0) {
            buf[len] = 0;
            // 요청 메소드와 경로 파싱
            sscanf(buf, "%s %s", method, path);
            printf("Request: %s %s\n", method, path);

            if (strstr(path, ".py")) { 
                // [POST] 파이썬 파일이면 CGI 실행 (읽은 데이터 buf도 같이 넘김)
                execute_cgi(clnt_sock, path + 1, method, buf);
            } else {
                // [GET] 아니면 파일 전송
                if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
                send_file(clnt_sock, path + 1);
                close(clnt_sock);
            }
        } else {
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}
