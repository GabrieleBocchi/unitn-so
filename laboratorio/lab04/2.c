#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    int fd = open("2.txt", O_RDONLY);
    FILE *file1, *file2;
    file1 = fopen("2c.txt", "w");
    file2 = fopen("2l.txt", "w");
    int charsRead;

    char c;
    charsRead = read(fd, &c, 1);
    while (charsRead != 0) {
        fputc(c, file1);
        charsRead = read(fd, &c, 1);
    }

    lseek(fd, 0, SEEK_SET);

    char line[200];
    charsRead = read(fd, line, sizeof(line));
    while (charsRead != 0) {
        fputs(line, file2);
        charsRead = read(fd, line, strlen(line));
    }

    close(fd);
    fclose(file1);
    fclose(file2);
    return 0;
}
