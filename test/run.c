
#include <stdio.h>

unsigned long long fibonacci(int n) {
    if (n <= 0) {
        return 0;
    } else if (n == 1) {
        return 1;
    } else {
        unsigned long long prev = 0;
        unsigned long long curr = 1;
        for (int i = 2; i <= n; ++i) {
            unsigned long long next = prev + curr;
            prev = curr;
            curr = next;
        }
        return curr;
    }
}

int main() {
    printf("First 100 Fibonacci numbers:\n");
    for (int i = 1; i <= 100; ++i) {
        printf("%llu ", fibonacci(i));
    }
    printf("\n");
    return 0;
}