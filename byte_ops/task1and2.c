#include <stdio.h>

// При вводе отрицательного числа выводятся все незначащие 1

void print_binary(unsigned int n) {
    int leading_zero = 1;
    for (int i = sizeof(n) * 8 - 1; i >= 0; i--) {
        if ((n >> i) & 1) {
            leading_zero = 0;
        }
        if (!leading_zero) {
            printf("%d", (n >> i) & 1);
        }
    }
    if (leading_zero) {
        printf("0");
    }
    printf("\n");
}

int main() {
    unsigned int num;
    printf("Введите положительное число:");
    scanf("%u", &num);

    printf("Двоичное представление: ");
    print_binary(num);

    return 0;
}