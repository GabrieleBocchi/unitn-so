#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *file;
int *sons;
int nSons;
int nSonsMax = 32;

void handlerUSR1(int sigNum) {
    int pid = fork();
    if (pid != 0) {
        printf("[server] +%d\n", pid);
        fprintf(file, "+%d\n", pid);
        fflush(file);
        sons[nSons] = pid;
        nSons++;
        if (nSons == nSonsMax) {
            nSonsMax *= 2;
            int *tmp = realloc(sons, nSonsMax * sizeof(int));
            if (tmp == NULL)
                exit(5);
            sons = tmp;
        }
    } else {
        fclose(file);
        pause();
    }
}

void handlerUSR2(int sigNum) {
    if (nSons == 0) {
        printf("[server] 0\n");
        fprintf(file, "0\n");
        fflush(file);
    } else {
        kill(sons[nSons - 1], SIGKILL);
        printf("[server] -%d\n", sons[nSons - 1]);
        fprintf(file, "-%d\n", sons[nSons - 1]);
        fflush(file);
        nSons--;
    }
}

void handlerINT(int sigNum) {
    fprintf(file, "%d\n", nSons);
    fflush(file);
    fclose(file);
    exit(0);
}

void serverFun(char *path) {
    file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        exit(3);
    }
    file = fopen(path, "w");
    if (file == NULL) {
        exit(4);
    }

    printf("[server:%d]\n", getpid());
    fprintf(file, "%d\n", getpid());
    fflush(file);
    signal(SIGUSR1, handlerUSR1);
    signal(SIGUSR2, handlerUSR2);
    signal(SIGINT, handlerINT);
    sons = malloc(nSonsMax * sizeof(int));
    nSons = 0;
    while (1)
        pause();
}

void clientFun(char *path) {
    FILE *file2;
    do {
        file2 = fopen(path, "r");
        sleep(1);
    } while (file2 == NULL);
    int serverPid;
    fscanf(file2, "%d", &serverPid);
    printf("[client] server: %d\n", serverPid);
    char c;
    int cont = 0;
    while ((c = getchar())) {
        if (c == '+') {
            if (cont < 10) {
                kill(serverPid, SIGUSR1);
                cont++;
                printf("[client] %d\n", cont);
            } else {
                printf("[client] %d\n", cont);
            }
        } else if (c == '-') {
            if (cont > 0) {
                kill(serverPid, SIGUSR2);
                cont--;
                printf("[client] %d\n", cont);
            } else {
                printf("[client] %d\n", cont);
            }
        } else if (c == '\n') {
            while (cont > 0) {
                kill(serverPid, SIGUSR2);
                cont--;
                printf("[client] %d\n", cont);
                sleep(1);
            }
            kill(serverPid, SIGINT);
            fclose(file2);
            exit(0);
        }
        while (getchar() != '\n')
            ;
    }
    fclose(file2);
    exit(0);
}

int main(int argc, char **argv) {
    if (argc != 3)
        return 1;

    if (strcmp(argv[1], "server") && strcmp(argv[1], "client"))
        return 2;

    if (strcmp(argv[1], "server") == 0)
        serverFun(argv[2]);
    else
        clientFun(argv[2]);

    return 0;
}
