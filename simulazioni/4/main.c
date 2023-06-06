#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

struct msgbuf {
    long mtype;
    char mtext[33];
} msg;

FILE *file;

void handlerUSR1(int sigNum, siginfo_t *info, void *context) {
    kill(info->si_pid, SIGUSR1);
    if (!fork()) {
        sleep(3);
        fprintf(file, "%d-%d\n", info->si_pid, SIGUSR1);
        fflush(file);
        exit(0);
    }
    return;
}

void handlerUSR2(int sigNum, siginfo_t *info, void *context) {
    int pid = info->si_pid;
    if (!fork()) {
        kill(pid, SIGUSR2);
        sleep(3);
        fprintf(file, "%d-%d\n", info->si_pid, SIGUSR2);
        fflush(file);
        exit(0);
    }
    return;
}

void handlerINT(int sigNum) {
    fprintf(file, "stop\n");
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 2)
        return 1;

    struct sigaction sa;
    sa.sa_sigaction = handlerUSR1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_sigaction = handlerUSR2;
    sigaction(SIGUSR2, &sa, NULL);

    signal(SIGINT, handlerINT);

    file = fopen(argv[1], "a");

    int key = ftok(argv[1], 1);
    int queueId = msgget(key, 0777 | IPC_CREAT);

    while (1) {
        msgrcv(queueId, &msg, sizeof(msg.mtext), 0, 0);
        kill(atoi(msg.mtext), SIGALRM);
    }

    return 0;
}
