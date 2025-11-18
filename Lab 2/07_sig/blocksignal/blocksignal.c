/*
 * 8장 시그널
 * 파일 이름: blocksignal.c
 * 기능: sigprocmask()를 이용한 시그널 차단
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h> // sleep()

int main()
{
    sigset_t set1, set2;

    /* 1. set1: 모든 시그널을 포함 */
    sigfillset(&set1);
    
    /* 2. set2: SIGINT(Ctrl+C)만 포함 */
    sigemptyset(&set2);
    sigaddset(&set2, SIGINT);

    printf("Critical region start.\n");
    /* 3. [모든 시그널 차단] 시작 */
    sigprocmask(SIG_BLOCK, &set1, NULL); 
    
    sleep(5);
    
    printf("Less critical region start.\n");
    /* 4. [SIGINT만 차단 해제] (다른 건 여전히 차단됨) */
    sigprocmask(SIG_UNBLOCK, &set2, NULL); 
    
    sleep(5);
    
    printf("Non critical region start.\n");
    /* 5. [모든 시그널 차단 해제] */
    sigprocmask(SIG_UNBLOCK, &set1, NULL); 

    sleep(5);
}
