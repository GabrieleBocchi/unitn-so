#include <stdio.h>
#include <string.h>

char *stringrev(char *str) {
    for (int i = 0; i < strlen(str) / 2; i++) {
        char temp = str[i];
        str[i] = str[strlen(str) - i - 1];
        str[strlen(str) - i - 1] = temp;
    }
    return str;
}

int stringpos(char *str, char chr) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == chr) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <string> <char>\n", argv[0]);
        return 1;
    }
    printf("Reverse of %s is: ", argv[1]);
    char str[20];
    strcpy(str, argv[1]);
    printf("%s\n", stringrev(str));
    printf("Position of %c in %s is: ", argv[2][0], argv[1]);
    if (stringpos(argv[1], argv[2][0]) == -1) {
        printf("Not found\n");
    } else {
        printf("%d\n", stringpos(argv[1], argv[2][0]));
    }
    return 0;
}
