#include <stdio.h>

int count_ones(unsigned int n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

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
    printf("Введите положительное число: ");
    scanf("%u", &num);

    printf("Двоичное представление: ");
    print_binary(num);

    printf("Количество единиц: %d\n", count_ones(num));

    return 0;
}