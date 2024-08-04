#include <stdio.h>
#include <pthread.h>

void *print_index(void *arg) {
    int index = *((int *)arg);
    printf("Thread index: %d\n", index);
    return NULL;
}

int main() {
    pthread_t threads[5];
    int t_id[5];

    for (int i = 0; i < 5; i++) {
        t_id[i] = i;
        pthread_create(&threads[i], NULL, print_index, &t_id[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
