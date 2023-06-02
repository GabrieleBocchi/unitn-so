#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
    printf("%zu\n", strlen(argv[1]));
    return 0;
}
