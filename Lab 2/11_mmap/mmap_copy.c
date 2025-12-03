/*
 * 9장 IPC / 과제 11
 * 파일 이름: mmap_copy.c
 * 기능: mmap을 이용한 고속 파일 복사 (Source -> Dest)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h> // mmap, munmap
#include <sys/stat.h> // fstat
#include <sys/types.h>

void fatal(const char *s) {
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    int fd_in, fd_out;
    void *src, *dst;
    struct stat statbuf;
    size_t length;

    if (argc != 3) {
        fprintf(stderr, "사용법: %s <원본 파일> <대상 파일>\n", argv[0]);
        exit(1);
    }

    /* 1. 원본 파일 열기 및 크기 확인 */
    if ((fd_in = open(argv[1], O_RDONLY)) < 0)
        fatal("원본 파일 열기 실패");
    
    if (fstat(fd_in, &statbuf) < 0) // fstat으로 파일 정보(크기) 획득
        fatal("fstat 실패");

    length = statbuf.st_size;

    /* 2. 대상 파일 생성 및 크기 설정 */
    if ((fd_out = open(argv[2], O_RDWR | O_CREAT, 0666)) < 0)
        fatal("대상 파일 생성 실패");
    
    if (ftruncate(fd_out, length) == -1) // 대상 파일 크기를 원본과 동일하게 설정
        fatal("ftruncate 실패");

    /* 3. 메모리 매핑: 원본 파일 (읽기 전용) */
    src = mmap(0, length, PROT_READ, MAP_SHARED, fd_in, 0);
    if (src == MAP_FAILED)
        fatal("원본 mmap 실패");
    close(fd_in);

    /* 4. 메모리 매핑: 대상 파일 (쓰기 전용) */
    dst = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
    if (dst == MAP_FAILED)
        fatal("대상 mmap 실패");
    close(fd_out);

    /* 5. 복사: 메모리에서 메모리로 (단 한 번의 memcpy) */
    memcpy(dst, src, length);

    /* 6. 메모리 매핑 해제 및 종료 */
    if (munmap(src, length) == -1 || munmap(dst, length) == -1)
        fatal("munmap 실패");
    
    printf("파일 복사 완료: %s -> %s (크기: %zu 바이트)\n", argv[1], argv[2], length);
    
    return 0;
}
