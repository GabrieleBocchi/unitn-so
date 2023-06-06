#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    char mtext[33];
} msg;

void sendSuccess(char *pid) {
    kill(atoi(pid), SIGUSR1);
    exit(0);
}

void sendError(char *pid) {
    kill(atoi(pid), SIGUSR2);
    exit(1);
}

int queueExists(char *name) {
    int key = ftok(name, 1);
    if (key == -1) {
        return 0;
    }
    int queueId = msgget(key, 0777);
    if (queueId) {
        return 1;
    } else {
        return 0;
    }
}

int getQueue(char *name) {
    int key = ftok(name, 1);
    return msgget(key, 0777);
}

int createQueue(char *name) {
    creat(name, 0777);
    int key = ftok(name, 1);
    return msgget(key, 0777 | IPC_CREAT);
}

int rcvMsg(int queueId) {
    return msgrcv(queueId, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);
}

int sndMsg(int queueId, char *message) {
    msg.mtype = 1;
    strcpy(msg.mtext, message);
    return msgsnd(queueId, &msg, sizeof(msg.mtext), IPC_NOWAIT);
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        printf("Usage: %s <name> <action> [<value>] <pid>\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[2], "new") == 0) {
        if (queueExists(argv[1])) {
            printf("%d", getQueue(argv[1]));
        } else {
            createQueue(argv[1]);
        }
        sendSuccess(argv[3]);
    } else if (strcmp(argv[2], "put") == 0) {
        int queueId = createQueue(argv[1]);
        int esito = sndMsg(queueId, argv[3]);
        if (!esito) {
            sendSuccess(argv[4]);
        } else {
            sendError(argv[4]);
        }
    } else if (strcmp(argv[2], "get") == 0) {
        int queueId = createQueue(argv[1]);
        int esito = rcvMsg(queueId);
        if (esito) {
            if (strcmp(msg.mtext, "") != 0) {
                printf("%s\n", msg.mtext);
            }
            sendSuccess(argv[3]);
        } else {
            sendError(argv[3]);
        }
    } else if (strcmp(argv[2], "del") == 0) {
        if (!queueExists(argv[1])) {
            sendError(argv[3]);
        }
        int queueId = getQueue(argv[1]);
        int esito = msgctl(queueId, IPC_RMID, NULL);
        remove(argv[1]);
        if (esito == -1) {
            sendError(argv[3]);
        } else {
            sendSuccess(argv[3]);
        }
    } else if (strcmp(argv[2], "emp") == 0) {
        int queueId = createQueue(argv[1]);
        int esito;
        do {
            esito = rcvMsg(queueId);
            if (esito != -1) {
                printf("%s\n", msg.mtext);
            }
        } while (esito != -1);
        sendSuccess(argv[3]);
    } else if (strcmp(argv[2], "mov") == 0) {
        if (!queueExists(argv[1])) {
            sendError(argv[4]);
        }
        int firstQueueId = getQueue(argv[1]);
        if (!queueExists(argv[3])) {
            createQueue(argv[3]);
        }
        int secondQueueId = getQueue(argv[3]);
        int cont = 0;
        struct msqid_ds info;
        msgctl(firstQueueId, IPC_STAT, &info);
        if (info.msg_qnum > 0) {
            int esitoRcv;
            do {
                int esitoSnd;
                esitoRcv = rcvMsg(firstQueueId);
                if (esitoRcv != -1) {
                    printf("%s\n", msg.mtext);
                    cont++;
                }
                esitoSnd = sndMsg(secondQueueId, msg.mtext);
                if (esitoSnd == -1) {
                    sendError(argv[4]);
                }
            } while (esitoRcv != -1);
        }
        printf("%d\n", cont);
        int esito = msgctl(firstQueueId, IPC_RMID, NULL);
        remove(argv[1]);
        if (esito == -1) {
            sendError(argv[4]);
        } else {
            sendSuccess(argv[4]);
        }
    } else {
        printf("Unknown action: %s\n", argv[2]);
        return 1;
    }
    return 0;
}
