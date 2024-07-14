#include <stdio.h>
#include "lib/calc.h"

void print_menu() {
    printf("1) Сложение\n");
    printf("2) Вычитание\n");
    printf("3) Умножение\n");
    printf("4) Деление\n");
    printf("5) Выход\n");
}

int main() {
    int choice, a, b, result;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        if (choice == 5) {
            break;
        }

        printf("Введите два числа: ");
        scanf("%d %d", &a, &b);

        switch (choice) {
            case 1:
                result = add(a, b);
                break;
            case 2:
                result = sub(a, b);
                break;
            case 3:
                result = mul(a, b);
                break;
            case 4:
                result = divide(a, b);
                break;
            default:
                printf("Неправильный выбор\n");
                continue;
        }

        printf("Результат: %d\n", result);
    }

    return 0;
}