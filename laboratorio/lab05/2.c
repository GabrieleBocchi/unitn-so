#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s <binary> <binary> <binary> ...\n", argv[0]);
        return 1;
    }
    int fd =
        open("outerr.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    dup2(fd, 1);
    dup2(fd, 2);

    for (int i = 1; i < argc; i++) {
        int isChild = !fork();
        wait(NULL);
        if (isChild) {
            char path[256] = "./";
            strcat(path, argv[i]);
            execv(path, NULL);
        }
    }

    close(fd);
    return 0;
}
