#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 3

void *hello_thread(void *arg) {
    // void 포인터를 int로 변환하여 출력
    printf("Thread %d: Hello, World!\n", (int)(long)arg);
    return arg;
}

int main() {
    pthread_t tid[NUM_THREADS];
    int i, status;

    // 3개의 쓰레드 생성
    for (i = 0; i < NUM_THREADS; i++) {
        status = pthread_create(&tid[i], NULL, hello_thread, (void *)(long)i);
        if (status != 0) {
            fprintf(stderr, "Create thread %d: %d", i, status);
            exit(1);
        }
    }

    pthread_exit(NULL);
}
