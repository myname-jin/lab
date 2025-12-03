#include <stdio.h>
#include <termios.h>    // tcgetattr, tcsetattr
#include <unistd.h>     // STDIN_FILENO
#include <wchar.h>      // wchar_t, wcslen, fgetwc, wprintf, putwchar
#include <locale.h>     // setlocale
#include <sys/time.h>   // gettimeofday
#include <stdlib.h>     // exit

// 1. 연습할 문장들 (한글 + 영문)
#define SENTENCE_COUNT 3
const wchar_t *sentences[SENTENCE_COUNT] = {
    L"동해물과 백두산이 마르고 닳도록",
    L"하느님이 보우하사 우리 나라 만세",
    L"Java와 Linux를 배우는 나는 동의대학교 임진호"
};

int main(void)
{
    int fd = STDIN_FILENO;
    struct termios init_attr, new_attr;
    wchar_t wch;
    
    int total_errors = 0;
    int total_chars = 0;
    
    struct timeval start, end;
    double elapsed_time;
    double wpm;

    setlocale(LC_ALL, "");

    // --- 터미널 설정 ---
    if (tcgetattr(fd, &init_attr) != 0) {
        perror("tcgetattr"); exit(1);
    }
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        perror("tcsetattr"); exit(1);
    }

    wprintf(L"--- 타자 연습 시작 (총 %d개 문장) ---\n\n", SENTENCE_COUNT);
    fflush(stdout);

    gettimeofday(&start, NULL);

    for (int i = 0; i < SENTENCE_COUNT; i++) 
    {
        const wchar_t *current_sentence = sentences[i];
        int len = wcslen(current_sentence);
        int current_pos = 0;

        wprintf(L"  %ls\n> ", current_sentence);
        fflush(stdout);
        
        // 4. 한 문장이 끝날 때까지 한 글자씩 입력받기
        while (current_pos < len) 
        {
            wch = fgetwc(stdin);

            if (wch == WEOF) {
                perror("fgetwc"); break;
            }

            // --- 해결 1: Enter 키 입력 무시 ---
            // Enter 키가 눌리면 아무것도 안 하고 다음 입력을 기다림
            if (wch == L'\n') {
                continue; 
            }
            
            // 4-1. 올바르게 입력한 경우
            if (wch == current_sentence[current_pos]) {
                putwchar(wch);
            } 
            // 4-2. 오타인 경우
            else {
                // --- 해결 2: 영문/한글 구분하여 별표 출력 ---
                wchar_t correct_char = current_sentence[current_pos];
                
                // 원본 글자가 ASCII(영문, 숫자, 1칸 기호)인지 확인
                if (correct_char < 128) {
                    putwchar(L'*'); // 1칸짜리 별표
                } else {
                    putwchar(L'＊'); // 2칸짜리 (한글용) 별표
                }
                total_errors++;
            }
            
            fflush(stdout);
            current_pos++;
            total_chars++;
        }
        wprintf(L"\n\n");
    }

    gettimeofday(&end, NULL);
    tcsetattr(fd, TCSANOW, &init_attr);

    // --- 최종 결과 계산 ---
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    elapsed_time = seconds + microseconds * 1e-6;

    wpm = (total_chars / 5.0) / (elapsed_time / 60.0);

    wprintf(L"--- 타자 연습 완료 ---\n");
    wprintf(L"총 소요 시간: %.2f초\n", elapsed_time);
    wprintf(L"총 오타 횟수: %d\n", total_errors);
    wprintf(L"평균 분당 타자수 (WPM): %.2f\n", wpm);

    return 0;
}
