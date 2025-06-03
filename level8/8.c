#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *global_str1 = "some_string_1";   // 0x8049ab0
char *global_str2 = "some_string_2";   // 0x8049aac
char *global_str3 = "some_string_3";   // 0x8049a80
char *global_str4 = "some_string_4";   // 0x8049aa0

int main() {
    printf(global_str1, global_str2);

    if (fgets((char[32]){0}, 128, stdin) == NULL)
        return 0;

    char input[32];
    fgets(input, 32, stdin);

    if (strncmp(input, "hello", 5) == 0) {
        global_str2 = malloc(4);
        *global_str2 = 0;
        if (strlen(input + 5) > 30) return 0;
        strcpy(global_str2, input + 5);
    }

    if (strncmp(input, "world", 5) == 0) {
        free(global_str2);
    }

    if (strncmp(input, "example", 6) == 0) {
        global_str1 = strdup(input + 7);
    }

    if (strncmp(input, "exit", 5) == 0) {
        if (global_str2[0] != 0) {
            system(global_str2);
            while(1) {}
        }
    }

    return 0;
}
