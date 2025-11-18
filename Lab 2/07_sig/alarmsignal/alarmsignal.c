/*
 * 8장 시그널
 * 파일 이름: alarmsignal.c
 * 기능: alarm()을 이용한 시그널 예약
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h> // alarm(), pause()
#include <stdlib.h>

void alarm_handler(int);
int alarm_flag = 0;

int main()
{
    struct sigaction act;
    
    act.sa_handler = alarm_handler;
    sigaction(SIGALRM, &act, NULL);
    
    alarm(5); /* 5초 뒤에 SIGALRM 시그널 예약 */

    pause();  /* 시그널이 올 때까지 멈춤 */

    if (alarm_flag)
        printf("Passed a 5 secs.\n");
}

void alarm_handler(int sig)
{
    printf("Received a alarm signal.\n");
    alarm_flag = 1;
}
