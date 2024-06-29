#include <stdio.h>

void print_matrix(int N) {
    int value = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", value++);
        }
        printf("\n");
    }
}

int main() {
    int N;
    printf("Введите размер матрицы N: ");
    scanf("%d", &N);
    print_matrix(N);
    return 0;
}