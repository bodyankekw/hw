#include <stdio.h>


int main() {
    enum {
        N = 8,
    };

    int arr[N];
    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < N / 2; i++) {
        int temp = arr[N - 1 - i];
        arr[N - 1 - i] = arr[i];
        arr[i] = temp;
    }

    for (int i = 0; i < N; i++){
        printf("%d ", arr[i]);
    }

    return 0;
}