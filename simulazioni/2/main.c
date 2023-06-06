#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *leafs;
int nLeafs;

void handlerUSR1Manager(int sigNum, siginfo_t *info, void *context) {
    union sigval value;
    value.sival_int = info->si_pid;
    sigqueue(leafs[nLeafs - 1], SIGUSR1, value);
    nLeafs--;
    if (nLeafs == 0) {
        exit(0);
    }
}

void handlerUSR1Leaf(int sigNum, siginfo_t *info, void *context) {
    kill(info->si_value.sival_int, SIGUSR2);
    exit(0);
}

void handlerTERM(int sigNum) {
    while (nLeafs > 0) {
        kill(leafs[nLeafs - 1], SIGKILL);
        nLeafs--;
    }
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        return 3;
    }

    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 10) {
        return 4;
    }

    FILE *file = fopen(argv[1], "r");
    if (file != NULL) {
        fclose(file);
        return 5;
    }
    file = fopen(argv[1], "w");
    if (file == NULL) {
        return 5;
    }

    fprintf(file, "%d\n", getpid());
    fflush(file);

    leafs = malloc(sizeof(int) * atoi(argv[2]));
    nLeafs = atoi(argv[2]);

    if (!fork()) {
        signal(SIGTERM, handlerTERM);

        sigset_t mod;
        sigaddset(&mod, SIGALRM);
        sigprocmask(SIG_BLOCK, &mod, NULL);

        struct sigaction sa;
        sa.sa_sigaction = handlerUSR1Manager;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &sa, NULL);

        fprintf(file, "%d\n", getpid());
        fflush(file);

        for (int i = 0; i < atoi(argv[2]); i++) {
            int pid = fork();
            if (!pid) {
                signal(SIGTERM, SIG_DFL);

                sa.sa_sigaction = handlerUSR1Leaf;
                sigemptyset(&sa.sa_mask);
                sa.sa_flags = SA_SIGINFO;
                sigaction(SIGUSR1, &sa, NULL);

                sigemptyset(&mod);
                sigaddset(&mod, SIGALRM);
                sigprocmask(SIG_UNBLOCK, &mod, NULL);

                sigemptyset(&mod);
                sigaddset(&mod, SIGCHLD);
                sigaddset(&mod, SIGCONT);
                sigprocmask(SIG_BLOCK, &mod, NULL);

                fprintf(file, "%d\n", getpid());
                fflush(file);

                fclose(file);
                pause();
            } else {
                leafs[i] = pid;
            }
        }
        fclose(file);
        while (1) {
            pause();
        }
    }
    fclose(file);
    return 0;
}
