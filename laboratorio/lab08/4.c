#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fd1, fd2, r;
    char buf[50];
    char *fifoName1 = "/tmp/fifo1";
    char *fifoName2 = "/tmp/fifo2";
    mkfifo(fifoName1, S_IRUSR | S_IWUSR);
    mkfifo(fifoName2, S_IRUSR | S_IWUSR);
    int child = !fork();
    if (child) {
        fd1 = open(fifoName1, O_RDONLY);
        fd2 = open(fifoName2, O_WRONLY);

        int cont = 1;

        while (1) {
            r = read(fd1, &buf, 50);
            buf[r] = '\0';
            printf("Child received:  ‘%s’\n", buf);
            sleep(1);
            if (strcmp(buf, "Stop this conversation!") == 0) {
                dprintf(fd2, "Ok, bye!");
                break;
            }
            dprintf(fd2, "Message %d from child!", cont);
            cont++;
        }

        close(fd1);
        close(fd2);
    } else {
        fd2 = open(fifoName1, O_WRONLY);
        fd1 = open(fifoName2, O_RDONLY);

        int cont = 1;

        while (1) {
            sleep(1);
            if (cont == 11) {
                dprintf(fd2, "Stop this conversation!");
                r = read(fd1, &buf, 50);
                buf[r] = '\0';
                printf("Parent received: ‘%s’\n", buf);
                break;
            }
            dprintf(fd2, "Message %d from parent!", cont);
            r = read(fd1, &buf, 50);
            buf[r] = '\0';
            printf("Parent received: ‘%s’\n", buf);
            cont++;
        }

        close(fd1);
        close(fd2);
    }
    wait(NULL);
    return 0;
}
