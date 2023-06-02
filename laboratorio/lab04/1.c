#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    FILE *file;
    file = fopen("1.txt", "r");
    int fd1 = open("1c.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    int fd2 = open("1l.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    char c;
    c = (char)fgetc(file);
    while (!feof(file)) {
        write(fd1, &c, sizeof(c));
        c = (char)fgetc(file);
    }

    rewind(file);

    char line[200];
    fgets(line, sizeof(line), file);
    while (!feof(file)) {
        write(fd2, line, strlen(line));
        fgets(line, sizeof(line), file);
    }

    fclose(file);
    close(fd1);
    close(fd2);
    return 0;
}
