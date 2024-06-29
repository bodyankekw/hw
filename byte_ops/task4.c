#include <stdio.h>

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
    unsigned int num, newByte;
    printf("Введите положительное число: ");
    scanf("%u", &num);

    printf("Двоичное представление: ");
    print_binary(num);

    printf("Введите значение для третьего байта (0-255): ");
    scanf("%u", &newByte);

    if (newByte > 255) {
        printf("Значение должно быть в диапазоне 0-255.\n");
        return 1;
    }

    num &= ~(0xFF << 16);
    num |= (newByte << 16);

    printf("Измененное число: %u\n", num);
    printf("Двоичное представление: ");
    print_binary(num);
    return 0;
}