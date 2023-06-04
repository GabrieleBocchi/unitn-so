#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int fd[2], isParent = 1, sum = 0;
    pipe(fd);
    char buf[50];
    for (int i = 0; i < atoi(argv[1]) && isParent; i++) {
        isParent = fork();
        if (!isParent) {
            srand(time(0) + getpid());
            int n = rand() % 100;
            dprintf(fd[1], "%d", n);
        } else {
            int r = read(fd[0], &buf, 50);
            buf[r] = '\0';
            printf("Child %d sent: %s\n", i + 1, buf);
            sum += atoi(buf);
        }
    }
    while (wait(NULL) > 0)
        ;
    if (isParent)
        printf("Sum: %d\n", sum);
    close(fd[0]);
    close(fd[1]);
    return 0;
}
