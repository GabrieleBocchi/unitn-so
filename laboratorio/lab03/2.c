#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char validArgs[8][2] = {"a", "b", "c", "d", "e", "f", "g", "h"};

int main(int argc, char **argv) {
    bool args[8] = {false, false, false, false, false, false, false, false};
    for (int i = 1; i < argc; i++) {
        for (int j = 0; j < 8; j++) {
            if (strcmp(argv[i], validArgs[j]) == 0) {
                args[j] = true;
                break;
            }
        }
    }
    printf("Valid args:\n");
    for (int i = 0; i < 8; i++)
        if (args[i] == true)
            printf("%s\n", validArgs[i]);
    return 0;
}
