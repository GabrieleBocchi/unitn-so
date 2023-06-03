#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <binary> <binary>\n", argv[0]);
        return 1;
    }
    int isChild = !fork();
    wait(NULL);
    if (isChild) {
        char path[256] = "./";
        strcat(path, argv[1]);
        int fd1 = open("/tmp/temp.txt", O_WRONLY | O_CREAT | O_TRUNC,
                       S_IRUSR | S_IWUSR);
        dup2(fd1, 1);
        execv(path, NULL);

        close(fd1);
    }
    char path[256] = "./";
    strcat(path, argv[2]);
    int fd2 = open("/tmp/temp.txt", O_RDONLY);
    dup2(fd2, 0);
    execv(path, NULL);

    close(fd2);
    return 0;
}
