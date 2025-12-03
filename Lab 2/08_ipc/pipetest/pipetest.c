/*
 * 9장 IPC
 * 파일 이름: pipetest.c
 * 기능: fork()와 pipe()를 이용한 부모-자식 간 통신
 */
#include <stdio.h>    // printf(), perror(), sprintf()
#include <stdlib.h>   // exit()
#include <unistd.h>   // pipe(), fork(), read(), write(), close()
#include <string.h>   // C언어 문자열 처리를 위해 (sprintf)

#define MSGSIZE 16

int main()
{
    char buf[MSGSIZE];
    int p[2], i;
    int pid;

    /* 1. 파이프 생성 (읽기용 p[0], 쓰기용 p[1]) */
    if (pipe(p) == -1) {
        perror("pipe call failed");
        exit(1);
    }

    pid = fork();

    if (pid == 0) {
        /* --- 자식 프로세스 (송신) --- */
        close(p[0]); // 자식은 읽기용 p[0]을 닫음
        
        /* "Hello" 메시지를 파이프(p[1])에 씀 */
        for (i = 0; i < 2; i++) {
            sprintf(buf, "Hello, world #%d", i+1);
            write(p[1], buf, MSGSIZE);
        }
    }
    else if (pid > 0) {
        /* --- 부모 프로세스 (수신) --- */
        close(p[1]); // 부모는 쓰기용 p[1]을 닫음
        
        /* 파이프(p[0])에서 메시지를 읽어와 출력 */
        for (i = 0; i < 2; i++) {
            read(p[0], buf, MSGSIZE);
            printf("%s\n", buf);
        }
    }
    else {
        perror("fork failed");
    }
}
