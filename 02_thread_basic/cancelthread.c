#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *cancel_thread(void *arg) {
    int i, state;
    for (i = 0;; i++) {
        // 취소 불가능 상태 설정
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
        printf("Thread: cancel state disabled\n");
        sleep(1);
        
        // 취소 가능 상태 복구
        pthread_setcancelstate(state, &state);
        printf("Thread: cancel state restored\n");
        
        // 취소 지점 생성을 위해 sleep 또는 testcancel 호출
        if (i % 5 == 0) pthread_testcancel();
    }
    return arg;
}

int main(int argc, char *argv[]) {
    pthread_t tid;
    int arg, status;
    void *result;

    if (argc < 2) {
        fprintf(stderr, "Usage: cancelthread time(sec)\n");
        exit(1);
    }

    // 쓰레드 생성
    status = pthread_create(&tid, NULL, cancel_thread, NULL);
    if (status != 0) {
        fprintf(stderr, "Create thread: %d", status);
        exit(1);
    }

    // 입력받은 시간만큼 대기 후 취소 요청
    arg = atoi(argv[1]);
    sleep(arg);

    status = pthread_cancel(tid);
    if (status != 0) {
        fprintf(stderr, "Cancel thread: %d", status);
        exit(1);
    }

    // 쓰레드 종료 대기
    status = pthread_join(tid, &result);
    if (status != 0) {
        fprintf(stderr, "Join thread: %d", status);
        exit(1);
    }

    //반환값 확인 
    return (int)(long)result;
}
