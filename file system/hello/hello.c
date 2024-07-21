#include <stdio.h>

int main() {
    const char filename[] = "output.txt";

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка при открытии файла для записи");
        return 1;
    }

    const char *text = "String from file";

    if (fputs(text, file) == EOF) {
        perror("Ошибка при записи в файл");
        fclose(file);
        return 1;
    }

    fclose(file);

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла для чтения");
        return 1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Ошибка при перемещении к концу файла");
        fclose(file);
        return 1;
    }

    long fileSize = ftell(file);
    if (fileSize == -1) {
        perror("Ошибка при определении размера файла");
        fclose(file);
        return 1;
    }

    printf("Содержимое файла с конца: ");
    for (long i = fileSize - 1; i >= 0; i--) {
        if (fseek(file, i, SEEK_SET) != 0) {
            perror("Ошибка при перемещении по файлу");
            fclose(file);
            return 1;
        }

        int ch = fgetc(file);
        if (ch == EOF) {
            perror("Ошибка при чтении файла");
            fclose(file);
            return 1;
        }

        putchar(ch);
    }

    printf("\n");

    fclose(file);

    return 0;
}
