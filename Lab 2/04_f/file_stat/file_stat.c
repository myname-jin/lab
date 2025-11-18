#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h> /* exit()를 위해 추가 */
#include <errno.h>  /* perror()를 위해 추가 */

int main(int argc, char *argv[])
{
    struct stat buf;
    char *msg;

    if (argc < 2) {
        fprintf(stderr,"Usage: file_stat filename\n");
        exit(1);
    }

    if (lstat(argv[1], &buf) < 0) {
        perror("lstat"); /* PDF와 달리 lstat을 사용 */
        exit(1);
    }

    if (S_ISREG(buf.st_mode)) msg = "regular file";
    else if (S_ISDIR(buf.st_mode)) msg = "directory";
    else if (S_ISCHR(buf.st_mode)) msg = "character special file";
    else if (S_ISBLK(buf.st_mode)) msg = "block special file";
    else if (S_ISFIFO(buf.st_mode)) msg = "pipe of FIFO";
    else if (S_ISLNK(buf.st_mode)) msg = "symbolic link";
    else if (S_ISSOCK(buf.st_mode)) msg = "socket";
    else msg = "unknown"; /* 이미지에 없는 'else' 추가 */

    printf("\"%s\" is %s\n", argv[1], msg);
}
