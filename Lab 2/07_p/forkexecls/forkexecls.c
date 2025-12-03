#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>   /* printf(), perror()를 위해 추가 */
#include <stdlib.h>  /* exit()를 위해 추가 */
#include <sys/wait.h> /* wait()를 위해 추가 */

int main()
{
    pid_t pid;
    pid = fork();
    
    if (pid == 0) {
        /* 자식 프로세스 */
        execl("/bin/ls", "ls", "-l", (char *) 0);
        
        /* execl()이 성공하면 이 아래 코드는 실행되지 않음 */
        perror("execl failed");

    } else if (pid > 0) {
        /* 부모 프로세스 */
        wait((int *) 0); /* 자식이 끝날 때까지 대기 */
        printf("ls completed\n");
        exit(0);
        
    } else {
        /* fork 실패 */
        perror("fork failed");
    }
}
