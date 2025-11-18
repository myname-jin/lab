#include <stdio.h>
#include <dlfcn.h>

int main() {
    void *handle = dlopen("./libarithmetic.so", RTLD_LAZY);
    if (!handle) {
        printf("Error: %s\n", dlerror());
        return 1;
    }
    
    int (*add)(int, int) = dlsym(handle, "add");
    int (*subtract)(int, int) = dlsym(handle, "subtract");
    int (*multiply)(int, int) = dlsym(handle, "multiply");
    int (*divide)(int, int) = dlsym(handle, "divide");
    
    int x = 10, y = 2;
    printf("add: %d\n", add(x, y));
    printf("subtract: %d\n", subtract(x, y));
    printf("multiply: %d\n", multiply(x, y));
    printf("divide: %d\n", divide(x, y));
    
    dlclose(handle);
    return 0;
}

