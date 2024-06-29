#include <stdio.h>


int main() {
    enum {
        N = 3,
    };

    int matrix[N][N];

    for(int i = 0; i < N; i++){
        for(int j = 0; j <= N - 1 - i; j++){
            matrix[i][j] = 0;
            matrix[N - j - 1][N - i - 1] = 1;
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}