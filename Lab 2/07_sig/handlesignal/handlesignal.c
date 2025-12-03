/*
 * 8장 시그널
 * 파일 이름: handlesignal.c
 * 기능: SIGINT 시그널 처리 (Ctrl+C 가로채기)
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h> // exit()를 위해 추가

void handler(int signo);

int main()
{
    struct sigaction act;
    int i = 0;

    /* 1. 핸들러 함수 지정 */
    act.sa_handler = handler;
    
    /* 2. 모든 시그널을 일단 차단하도록 설정 (핸들러 실행 중) */
    sigfillset(&(act.sa_mask));
    
    /* 3. SIGINT(Ctrl+C)에 대해 act 설정 적용 */
    sigaction(SIGINT, &act, NULL);
    
    printf("SIGINT on\n");
    
    while(1) {
        sleep(1);
        printf("sleep for %d sec(s).\n", ++i);
    }
}

/* 시그널 핸들러 함수 */
void handler(int signo)
{
    printf("handler: signo=%d\n", signo);
}
