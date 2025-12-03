#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// 공유 자원 및 동기화 도구
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// 이진 플래그 (0: 부모 차례, 1: 자식 차례)
int flag = 0; 

// 반복 횟수 설정
#define ITERATIONS 5

void *child_thread(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);

        // 내 차례(flag == 1)가 아니면 대기
        while (flag != 1) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("hello child\n");
        sleep(1); // 1초 대기

        // 턴을 부모(0)에게 넘김
        flag = 0;
        pthread_cond_signal(&cond); // 대기 중인 부모 깨우기
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t tid;

    // 자식 쓰레드 생성
    if (pthread_create(&tid, NULL, child_thread, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    // 부모 쓰레드 로직
    for (int i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);

        // 내 차례(flag == 0)가 아니면 대기
        while (flag != 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        printf("hello parent\n");
        sleep(1); // 1초 대기

        // 턴을 자식(1)에게 넘김
        flag = 1;
        pthread_cond_signal(&cond); // 대기 중인 자식 깨우기
        pthread_mutex_unlock(&mutex);
    }

    // 자식 쓰레드 종료 대기
    pthread_join(tid, NULL);
    
    return 0;
}
