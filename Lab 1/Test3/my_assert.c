#include <stdio.h>
#include <stdlib.h>

void my_assert(int expression, const char *expr_str, const char *file, int line) {
    if (!expression) {
        fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", expr_str, file, line);
        abort();
    }
}

// 사용 예시 (매크로로 편리하게)
#define MY_ASSERT(expr) my_assert((expr), #expr, __FILE__, __LINE__)

int main() {
    int x = 0;
    MY_ASSERT(x != 0);  // 거짓이므로, assert 에러 메시지가 뜸
    return 0;
}

