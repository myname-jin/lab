#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// 쓰레드 함수
void *hello_thread(void *arg) {
    printf("Thread: Hello, World!\n");
    return arg;
}

int main() {
    pthread_t tid;
    int status;

    // 쓰레드 생성
    status = pthread_create(&tid, NULL, hello_thread, NULL);
    if (status != 0) {
        perror("Create thread");
        exit(1);
    }

    // 메인 쓰레드 종료 (생성된 쓰레드가 실행될 시간을 벌어줌)
    pthread_exit(NULL);
}
