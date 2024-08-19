#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME "/my_queue"
#define MAX_SIZE 1024

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];

    mq = mq_open(QUEUE_NAME, O_RDONLY);
    if (mq == (mqd_t) -1) {
        perror("Client: mq_open");
        exit(1);
    }

    if (mq_receive(mq, buffer, MAX_SIZE, NULL) == -1) {
        perror("Client: mq_receive");
        exit(1);
    }
    printf("Client: Received message: %s\n", buffer);

    mq_close(mq);

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t) -1) {
        perror("Client: mq_open for sending");
        exit(1);
    }

    strcpy(buffer, "Hello!");
    if (mq_send(mq, buffer, MAX_SIZE, 0) == -1) {
        perror("Client: mq_send");
        exit(1);
    }
    printf("Client: Sent message: %s\n", buffer);

    mq_close(mq);

    return 0;
}
