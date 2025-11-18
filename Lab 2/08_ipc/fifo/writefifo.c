/*
 * 9장 IPC
 * 파일 이름: writefifo.c (송신측)
 */
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>   // write()
#include <stdlib.h>   // exit()
#include <string.h>   // strcpy()

#define MSGSIZE 64

char *testfifo = "fifo"; // 접속할 파일 이름

int main (int argc, char **argv)
{
    int fd, i, nwrite;
    char buf[MSGSIZE];

    if (argc < 2) {
        fprintf(stderr, "Usage: ./writefifo msg ...\n");
        exit(1);
    }

    /* 1. "fifo" 파일을 (쓰기 전용) 모드로 열기 */
    if ((fd = open(testfifo, O_WRONLY)) < 0) {
        perror("fifo open failed");
        exit(1);
    }

    /* 2. 프로그램 실행 시 입력받은 인자(argv)들을 "fifo"에 쓴다 */
    for (i = 1; i < argc; i++) {
        strcpy(buf, argv[i]);
        if ((nwrite = write(fd, buf, MSGSIZE)) < 0) {
            perror("fifo write failed");
            exit(1);
        }
    }
}
