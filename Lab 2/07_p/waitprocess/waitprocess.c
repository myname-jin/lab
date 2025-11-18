#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> /* printf(), perror()를 위해 추가 */

int main()
{
    pid_t pid;
    int status, exit_status;

    if ((pid = fork()) < 0) {
        perror("fork failed");
    }

    if (pid == 0) {
        /* 자식 코드 */
        sleep(4); /* 4초 동안 중단 */
        exit(5);  /* 5라는 종료 상태를 남기고 종료 */
    }

    /* --- 여기서부터 부모 코드 --- */
    
    /* 자식이 퇴장(exit)할 때까지 대기 */
    if ((pid = wait(&status)) == -1) {
        perror("wait failed");
        exit(2);
    }

    /* 자식이 정상적으로 종료되었는지 확인 */
    if (WIFEXITED(status)) {
        /* 자식이 남긴 종료 상태(5)를 추출 */
        exit_status = WEXITSTATUS(status);
        printf("Exit status from %d was %d\n",
            pid, exit_status);
    }
    
    exit(0);
}
