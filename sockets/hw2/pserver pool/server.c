#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_THREADS 5
#define BUFFER_SIZE 1024

typedef struct {
    int client_socket;
    int id;
} thread_arg;

int pipe_fd[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int available_threads[MAX_THREADS];
int thread_count = MAX_THREADS;

void *handle_client(void *arg) {
    thread_arg *t_arg = (thread_arg *)arg;
    int sock = t_arg->client_socket;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("Thread %d is handling client\n", t_arg->id);

    while ((bytes_read = read(sock, buffer, sizeof(buffer))) > 0) {
        printf("Thread %d received: %s\n", t_arg->id, buffer);
        send(sock, buffer, bytes_read, 0);
    }

    close(sock);

    pthread_mutex_lock(&mutex);
    write(pipe_fd[1], &t_arg->id, sizeof(int));
    pthread_mutex_unlock(&mutex);

    free(t_arg);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);

    if (pipe(pipe_fd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    for (int i = 0; i < MAX_THREADS; i++) {
        available_threads[i] = i;
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }

        printf("New connection accepted\n");

        pthread_mutex_lock(&mutex);
        while (thread_count == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        int thread_id = available_threads[--thread_count];
        pthread_mutex_unlock(&mutex);

        thread_arg *t_arg = malloc(sizeof(thread_arg));
        t_arg->client_socket = new_socket;
        t_arg->id = thread_id;

        if (pthread_create(&threads[thread_id], NULL, handle_client, (void *)t_arg) != 0) {
            perror("Failed to create thread");
            close(new_socket);
            free(t_arg);
            continue;
        }

        int finished_thread_id;
        read(pipe_fd[0], &finished_thread_id, sizeof(int));

        pthread_mutex_lock(&mutex);
        available_threads[thread_count++] = finished_thread_id;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    close(server_fd);
    pthread_mutex_destroy(&mutex);
    return 0;
}
