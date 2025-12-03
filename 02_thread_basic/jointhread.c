#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *join_thread(void *arg) {
    int ret = (int)(long)arg; // 인자로 받은 값을 반환값으로 사용
    pthread_exit((void *)(long)ret); 
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    int arg, status;
    void *result;

    if (argc < 2) {
        fprintf(stderr, "Usage: jointhread <number>\n");
        exit(1);
    }

    arg = atoi(argv[1]);

    // 쓰레드 생성
    status = pthread_create(&tid, NULL, join_thread, (void *)(long)arg);
    if (status != 0) {
        fprintf(stderr, "Create thread: %d", status);
        exit(1);
    }

    // 쓰레드 종료 대기 (Join)
    status = pthread_join(tid, &result);
    if (status != 0) {
        fprintf(stderr, "Join thread: %d", status);
        exit(1);
    }

    // 결과 확인 (반환값은 쉘의 종료 코드로 사용됨)
    return (int)(long)result;
}
