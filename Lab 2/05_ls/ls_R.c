/*
 * 7장 파일 처리 (재귀적 디렉토리 탐색)
 * 파일 이름: ls_R.c
 * 기능: ls -R 명령어 구현
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>     // stat 함수와 함께 사용
#include <dirent.h>     // opendir, readdir, closedir
#include <string.h>     // strcmp, snprintf
#include <errno.h>      // perror

// 함수 선언
void list_directory(const char *path);

int main(int argc, char *argv[]) {
    const char *start_path = "."; // 기본 경로는 현재 디렉토리

    // 명령행 인자로 디렉토리 경로를 제공받으면 그 경로로 시작
    if (argc > 1) {
        start_path = argv[1];
    }
    
    // 디렉토리 나열 시작
    list_directory(start_path);
    return 0;
}

void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;

    // 1. 디렉토리를 열기 
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    printf("\n%s:\n", path); // 현재 디렉토리 경로 출력

    // 2. 디렉토리 내용을 읽으며 파일/디렉토리 이름 출력 
    while ((entry = readdir(dir)) != NULL) {
        // 숨김 파일 및 현재/상위 디렉토리 제외
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        printf("%s  ", entry->d_name); // 파일/디렉토리 이름 출력
    }
    printf("\n");

    // 3. 다시 디렉토리 스트림을 읽기 위해 포인터를 처음으로 되돌림 
    rewinddir(dir);

    // 4. 하위 디렉토리를 찾기 위해 다시 반복
    while ((entry = readdir(dir)) != NULL) {
        char sub_path[1024];

        // 숨김 파일 및 현재/상위 디렉토리 제외
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 전체 경로 생성 (예: "./test_dir")
        snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
        
        struct stat statbuf;

        // 5. 파일 정보를 가져옴 
        if (stat(sub_path, &statbuf) == 0) {
            // 6. 항목이 디렉토리인지 확인 
            if (S_ISDIR(statbuf.st_mode)) {
                // 7. 재귀 호출: 하위 디렉토리 탐색
                list_directory(sub_path);
            }
        } else {
            perror("stat");
        }
    }
    
    // 8. 디렉토리 닫기 
    closedir(dir);
}
