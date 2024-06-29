#include <stdio.h>

int main() {
    enum {
        N = 10,
    };

    int arr[N];
    int *ptr = &arr;

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < N; ++i) {
        printf("%d ", *(ptr + i));
    }
    return 0;
}
