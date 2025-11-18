#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h> /* exit()를 위해 추가 */

int main(int argc, char *argv[])
{
    int fd;

    if (argc < 2) {
        fprintf(stderr,"Usage: file_check filename\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }

    /* 문법 오류 수정: "File %s found...\n" */
    printf("File \"%s\" found...\n", argv[1]);
    
    close(fd);
}
