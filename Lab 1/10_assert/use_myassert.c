#include <stdio.h>
#include <stdlib.h>


void my_assert(int expression, const char *expr_str, const char *file, int line) {
    if (!expression) {
        fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", expr_str, file, line);
        abort();
    }
}

#define MY_ASSERT(expr) my_assert((expr), #expr, __FILE__, __LINE__)

int main(int argc, char *argv[]) {
    FILE *f;

    MY_ASSERT(argc >= 2);  // assert 대체

    if ((f = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    printf("Open a file \"%s\".\n", argv[1]);
    fclose(f);
}

