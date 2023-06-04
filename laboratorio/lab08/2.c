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

        int cont = 1;

        while (1) {
            r = read(pipe1[READ], &buf, 50);
            buf[r] = '\0';
            printf("Child received:  ‘%s’\n", buf);
            sleep(1);
            if (strcmp(buf, "Stop this conversation!") == 0) {
                dprintf(pipe2[WRITE], "Ok, bye!");
                break;
            }
            dprintf(pipe2[WRITE], "Message %d from child!", cont);
            cont++;
        }

        close(pipe1[READ]);
        close(pipe2[WRITE]);
    } else {
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        int cont = 1;

        while (1) {
            sleep(1);
            if (cont == 11) {
                dprintf(pipe1[WRITE], "Stop this conversation!");
                r = read(pipe2[READ], &buf, 50);
                buf[r] = '\0';
                printf("Parent received: ‘%s’\n", buf);
                break;
            }
            dprintf(pipe1[WRITE], "Message %d from parent!", cont);
            r = read(pipe2[READ], &buf, 50);
            buf[r] = '\0';
            printf("Parent received: ‘%s’\n", buf);
            cont++;
        }

        close(pipe1[WRITE]);
        close(pipe2[READ]);
    }
    wait(NULL);
    return 0;
}
