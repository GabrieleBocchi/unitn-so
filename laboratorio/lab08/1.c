#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main() {
    int pipe1[2], pipe2[2], r;
    pipe(pipe1);
    pipe(pipe2);
    char buf[50];
    int child = !fork();
    if (child) {
        close(pipe1[WRITE]);
        close(pipe2[READ]);

        r = read(pipe1[READ], &buf, 50);
        buf[r] = '\0';
        printf("Child received:  ‘%s’\n", buf);
        sleep(1);
        dprintf(pipe2[WRITE], "Message 1 from child!");

        r = read(pipe1[READ], &buf, 50);
        buf[r] = '\0';
        printf("Child received:  ‘%s’\n", buf);
        sleep(1);
        dprintf(pipe2[WRITE], "Message 2 from child!");

        close(pipe1[READ]);
        close(pipe2[WRITE]);
    } else {
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        sleep(1);
        dprintf(pipe1[WRITE], "Message 1 from parent!");
        r = read(pipe2[READ], &buf, 50);
        buf[r] = '\0';
        printf("Parent received: ‘%s’\n", buf);

        sleep(1);
        dprintf(pipe1[WRITE], "Message 2 from parent!");
        r = read(pipe2[READ], &buf, 50);
        buf[r] = '\0';
        printf("Parent received: ‘%s’\n", buf);

        close(pipe1[WRITE]);
        close(pipe2[READ]);
    }
    wait(NULL);
    return 0;
}
