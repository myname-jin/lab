/*
 * 9장 IPC / 과제 10
 * 파일 이름: mq_chat.c
 * 기능: 메시지 큐를 이용한 양방향 채팅 프로그램 (요청-응답 모델)
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFSIZE 1024 // 안전하게 버퍼 크기를 1024로 설정
#define MSG_TYPE_A 1 // 사용자 1의 메시지 유형
#define MSG_TYPE_B 2 // 사용자 2의 메시지 유형

// 메시지 구조체 (PDF 47페이지 기반)
struct msg_buffer {
    long msg_type;
    char text[BUFSIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("사용법: %s [송신 유형] [수신 유형]\n", argv[0]);
        printf("예: ./mq_chat 1 2 (유형 1로 보내고 유형 2를 받음)\n");
        return 1;
    }
    
    // 1. 송신/수신 메시지 유형 설정
    long send_type = atol(argv[1]);
    long receive_type = atol(argv[2]);

    // 2. 메시지 큐 Key 생성 (ftok)
    // 현재 디렉토리(".")의 inode와 '65'를 조합해 Key를 만듭니다.
    key_t key = ftok(".", 65);
    
    // 3. 메시지 큐 생성 및 접근 (IPC_CREAT | 0666)
    int msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("msgget failed");
        return 1;
    }

    struct msg_buffer msg;
    char message[BUFSIZE];

    printf("--- 메시지 큐 채팅 시작 ---\n");
    printf("현재 사용자: 송신 유형 %ld, 수신 유형 %ld\n", send_type, receive_type);
    printf("종료하려면 'exit'을 입력하세요.\n");
    
    while (1) {
        // --- 메시지 전송 (키보드 입력) ---
        printf("나> ");
        fflush(stdout);
        
        // 4. 키보드 입력
        if (fgets(message, sizeof(message), stdin) == NULL) break;
        message[strcspn(message, "\n")] = '\0'; // 개행 문자 제거

        // 5. 종료 확인 및 전송
        if (strcmp(message, "exit") == 0) {
            // 상대방이 기다리고 있으므로, 종료 메시지를 보낸 후 break
            msg.msg_type = send_type;
            strncpy(msg.text, message, sizeof(msg.text));
            msgsnd(msg_id, &msg, strlen(msg.text) + 1, 0);
            printf("채팅을 종료합니다.\n");
            break;
        }

        // 6. 메시지 전송 (msgsnd)
        msg.msg_type = send_type;
        strncpy(msg.text, message, sizeof(msg.text));
        if (msgsnd(msg_id, &msg, strlen(msg.text) + 1, 0) == -1) {
            perror("msgsnd failed");
            break;
        }

        // --- 메시지 수신 (상대방 응답 대기) ---
        // 7. 수신 (msgrcv) - 상대방이 보낼 때까지 멈춰서 기다림
        if (msgrcv(msg_id, &msg, sizeof(msg.text), receive_type, 0) == -1) {
            if (errno == EIDRM) { // 상대방이 큐를 삭제하고 종료했을 경우
                printf("상대방이 채팅을 종료했습니다.\n");
                break;
            }
            perror("msgrcv failed");
            break;
        }
        
        // 8. 수신된 메시지 출력
        printf("상대방> %s\n", msg.text);

        // 9. 상대방이 'exit'을 보냈다면 종료
        if (strcmp(msg.text, "exit") == 0) {
            printf("상대방이 채팅을 종료했습니다.\n");
            break;
        }
    }
    
    // 10. 1번 사용자(send_type=1)가 종료할 때만 메시지 큐를 삭제
    if (send_type == 1) { 
        printf("메시지 큐(ID %d)를 시스템에서 삭제합니다.\n", msg_id);
        msgctl(msg_id, IPC_RMID, NULL); 
    }
    return 0;
}
