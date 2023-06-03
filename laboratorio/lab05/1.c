#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s <binary> <binary> <binary> ...\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        int isChild = !fork();
        wait(NULL);
        if (isChild) {
            char path[256] = "./";
            strcat(path, argv[i]);
            execv(path, NULL);
        }
    }
    return 0;
}
