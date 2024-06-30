#include <stdio.h>
#include <string.h>

#define MAX_ABONENTS 100
#define NAME_LEN 10

struct abonent {
    char name[NAME_LEN];
    char second_name[NAME_LEN];
    char tel[NAME_LEN];
};

void add_abonent(struct abonent list[], int *count);

void delete_abonent(struct abonent list[], int *count);

void search_abonent(struct abonent list[], int count);

void print_abonents(struct abonent list[], int count);

int main() {
    struct abonent list[MAX_ABONENTS] = {0};
    int abonents_count = 0;
    int choice;

    do {
        printf("\nМеню:\n");
        printf("1) Добавить абонента\n");
        printf("2) Удалить абонента\n");
        printf("3) Поиск абонентов по имени\n");
        printf("4) Вывод всех записей\n");
        printf("5) Выход\n");
        printf("Введите номер пункта: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_abonent(list, &abonents_count);
                break;
            case 2:
                delete_abonent(list, &abonents_count);
                break;
            case 3:
                search_abonent(list, abonents_count);
                break;
            case 4:
                print_abonents(list, abonents_count);
                break;
            case 5:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неправильный ввод. Попробуйте еще раз.\n");
        }
    } while (choice != 5);

    return 0;
}

void add_abonent(struct abonent list[], int *count) {
    if (*count >= MAX_ABONENTS) {
        printf("Справочник переполнен. Невозможно добавить больше абонентов.\n");
        return;
    }

    printf("Введите имя: ");
    scanf("%s", list[*count].name);
    printf("Введите фамилию: ");
    scanf("%s", list[*count].second_name);
    printf("Введите телефон: ");
    scanf("%s", list[*count].tel);

    (*count)++;
}


void delete_abonent(struct abonent list[], int *count) {
    char name[NAME_LEN];
    printf("Введите имя абонента для удаления: ");
    scanf("%s", name);

    for (int i = 0; i < *count; i++) {
        if (strcmp(list[i].name, name) == 0) {
            for (int j = i; j < *count - 1; j++) {
                list[j] = list[j + 1];
            }
            memset(&list[*count - 1], 0, sizeof(struct abonent));
            (*count)--;
            printf("Абонент удален.\n");
            return;
        }
    }
    printf("Абонент не найден.\n");
}

void search_abonent(struct abonent list[], int count) {
    char name[NAME_LEN];
    printf("Введите имя для поиска: ");
    scanf("%s", name);

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].name, name) == 0) {
            printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
                   list[i].name, list[i].second_name, list[i].tel);
        }
    }
}

void print_abonents(struct abonent list[], int count) {
    for (int i = 0; i < count; i++) {
        printf("Имя: %s, Фамилия: %s, Телефон: %s\n",
               list[i].name, list[i].second_name, list[i].tel);
    }
}