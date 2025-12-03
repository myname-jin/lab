/*
 * 9장 IPC
 * 파일 이름: readfifo.c (수신측)
 */
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>   // read()
#include <stdlib.h>   // exit()
#include <sys/types.h> // mkfifo()
#include <sys/stat.h>  // mkfifo()

#define MSGSIZE 64

char *testfifo = "fifo"; // 통신에 사용할 파일 이름

int main (int argc, char **argv)
{
    int fd;
    char buf[MSGSIZE];
    
    /* 1. "fifo"라는 이름의 명명된 파이프 생성 */
    if (mkfifo(testfifo, 0666) == -1) {
        perror("mkfifo failed");
        // exit(1); // PDF에는 있지만, "File exists" 오류 시에도 계속 진행하기 위해 주석 처리
    }

    /* 2. "fifo" 파일을 (읽기+쓰기) 모드로 열기 */
    if ((fd = open(testfifo, O_RDWR)) < 0) {
        perror("fifo open failed");
        exit(1);
    }

    /* 3. "fifo" 파일에서 메시지를 계속 읽어와 출력 */
    while (1) {
        if (read(fd, buf, MSGSIZE) < 0) {
            perror("fifo read failed");
            exit(1);
        }
        printf("received message: %s\n", buf);
    }
}
