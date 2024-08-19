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

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("Client: msgget");
        exit(1);
    }

    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
        perror("Client: msgrcv");
        exit(1);
    }
    printf("Client: Received message: %s\n", message.msg_text);

    message.msg_type = 2;
    strcpy(message.msg_text, "Hello!");
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("Client: msgsnd");
        exit(1);
    }
    printf("Client: Sent message: %s\n", message.msg_text);

    return 0;
}
