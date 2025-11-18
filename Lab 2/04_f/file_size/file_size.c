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
    off_t size;

    if (argc < 2) {
        fprintf(stderr,"Usage: file_size filename\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1){
        perror("open");
        exit(1);
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        exit(1);
    }

    /* off_t 타입은 long int일 수 있으므로 %ld 사용 */
    printf("%s's size = %ld\n", argv[1], size);

    close(fd);
}
