/*
 * 9장 IPC
 * 파일 이름: testsem.c (Sys V 세마포어)
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>   // exit(), srand(), rand()
#include <unistd.h>   // fork(), getpid(), sleep()

#define SEMKEY (key_t)0111 // 세마포어를 식별하기 위한 키

/* 함수 선언 */
void testsem(int semid);
void p(int semid);
void v(int semid);

/* * main 함수 (부모): 세마포어를 만들고,
 * 자식 3개를 생성한 뒤, 10초 후 세마포어를 제거한다.
 */
int main()
{
    int semid, i;
    union semun {
        int value;
        struct semid_ds *buf;
        unsigned short int *array;
    } arg; // semctl에 값을 전달하기 위한 공용체

    /* 1. 세마포어 1개를 생성 (없으면 IPC_CREAT) */
    if ((semid = semget(SEMKEY, 1, IPC_CREAT | 0666)) == -1) {
        perror("semget failed");
        exit(1);
    }

    /* 2. 세마포어 0번의 값을 1로 초기화 (1명만 통과 가능) */
    arg.value = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl failed");
        exit(1);
    }

    /* 3. 자식 프로세스 3개 생성 */
    for (i = 0; i < 3; i++) {
        if (!fork()) // fork() == 0, 즉 자식 프로세스라면
            testsem(semid); // testsem 함수 실행
    }

    sleep(10); // 자식들이 모두 끝날 때까지 10초 대기

    /* 4. 세마포어 제거(IPC_RMID) */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        perror("semctl failed");
        exit(1);
    }
}

/* * testsem 함수 (자식): 세마포어(lock)를 획득하고,
 * 랜덤 시간 동안 "작업"한 뒤, 세마포어를 반납한다.
 */
void testsem(int semid)
{
    srand((unsigned int)getpid());

    p(semid); // "잠금" (P 연산)
    printf("process %d: semaphore in use\n", getpid());
    
    sleep(rand() % 5); // 0~4초간 "작업" (sleep)
    
    printf("process %d: putting semaphore\n", getpid());
    v(semid); // "해제" (V 연산)
    
    exit(0);
}

/* P 연산: 세마포어 값을 1 감소 (잠금 요청) */
void p(int semid)
{
    struct sembuf pbuf;
    pbuf.sem_num = 0;   // 0번 세마포어
    pbuf.sem_op = -1;  // 값을 1 감소
    pbuf.sem_flg = SEM_UNDO;
    if (semop(semid, &pbuf, 1) == -1) {
        perror("semop p failed");
        exit(1);
    }
}

/* V 연산: 세마포어 값을 1 증가 (잠금 해제) */
void v(int semid)
{
    struct sembuf vbuf;
    vbuf.sem_num = 0;
    vbuf.sem_op = 1;   // 값을 1 증가
    vbuf.sem_flg = SEM_UNDO;
    if (semop(semid, &vbuf, 1) == -1) {
        perror("semop v failed");
        exit(1);
    }
}
