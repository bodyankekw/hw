#include <stdio.h>
#include <string.h>

char *find_substring(const char *str, const char *substr) {
    while (*str) {
        const char *begin = str;
        const char *pattern = substr;

        while (*str && *pattern && *str == *pattern) {
            str++;
            pattern++;
        }

        if (!*pattern) {
            return (char *) begin;
        }

        str = begin + 1;
    }
    return NULL;
}

int main() {
    enum {
        N = 100,
    };

    char str[N], substr[N];

    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    printf("Введите подстроку: ");
    fgets(substr, sizeof(substr), stdin);
    substr[strcspn(substr, "\n")] = '\0';

    char *result = find_substring(str, substr);

    if (result) {
        printf("Подстрока найдена: %s\n", result);
    } else {
        printf("Подстрока не найдена.\n");
    }

    return 0;
}