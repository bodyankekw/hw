#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_SIZE 1024

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_SIZE];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    key = ftok("server", 'A');

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Server: msgget");
        exit(1);
    }

    message.msg_type = 1;
    strcpy(message.msg_text, "Hi!");
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("Server: msgsnd");
        exit(1);
    }
    printf("Server: Sent message: %s\n", message.msg_text);

    if (msgrcv(msgid, &message, sizeof(message.msg_text), 2, 0) == -1) {
        perror("Server: msgrcv");
        exit(1);
    }
    printf("Server: Received message: %s\n", message.msg_text);

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("Server: msgctl");
        exit(1);
    }

    return 0;
}
