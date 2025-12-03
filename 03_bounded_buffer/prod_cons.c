#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 5       // 버퍼 크기
#define ITEMS 10    // 처리 아이템 수

int buffer[MAX];
int in = 0, out = 0, count = 0; // 공유 자원

// 정적 초기화
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < ITEMS; i++) {
        usleep(rand() % 10000); // 생산 시간 시뮬레이션
        
        pthread_mutex_lock(&mutex);
        while (count == MAX) { // 버퍼가 꽉 찼으면 대기
            printf("[Producer %ld] Buffer Full. Waiting...\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }
        
        buffer[in] = rand() % 100;
        in = (in + 1) % MAX;
        count++;
        printf("[Producer %ld] Inserted: %d (Count: %d)\n", id, buffer[(in-1+MAX)%MAX], count);
        
        pthread_cond_signal(&not_empty); // 비어있지 않다고 알림
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    long id = (long)arg;
    for (int i = 0; i < ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) { // 버퍼가 비었으면 대기
            printf("\t<Consumer %ld> Buffer Empty. Waiting...\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }
        
        int item = buffer[out];
        out = (out + 1) % MAX;
        count--;
        printf("\t<Consumer %ld> Removed: %d (Count: %d)\n", id, item, count);
        
        pthread_cond_signal(&not_full); // 꽉 차지 않았다고 알림
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 10000); // 소비 시간 시뮬레이션
    }
    return NULL;
}

int main() {
    pthread_t t[4]; // 0,1: Producer / 2,3: Consumer
    srand(time(NULL));

    // 쓰레드 생성 (생산자 2명, 소비자 2명)
    for (long i = 0; i < 4; i++) {
        if (i < 2) pthread_create(&t[i], NULL, producer, (void*)i);
        else       pthread_create(&t[i], NULL, consumer, (void*)i);
    }

    // 쓰레드 대기
    for (int i = 0; i < 4; i++) pthread_join(t[i], NULL);

    printf("All threads finished.\n");
    return 0;
}
