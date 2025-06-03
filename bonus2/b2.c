#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int greet_style = 0;

const char data0[] = "Hello! ";
const char data1[] = "Greetings, user! ";
const char data2[] = "Hi there! ";

void frame_dummy() {
    void (*func_ptr)(void*) = *(void (**)(void*))0x8049880;
    if (func_ptr) {
        func_ptr((void*)0x8049880);
    }
}

void greetuser(char *inputStr) {
    char buffer[88] = {0};

    if (greet_style == 1) {
        memcpy(buffer, data1, sizeof(data1) - 1);
    }
    else if (greet_style == 2) {
        memcpy(buffer, data2, sizeof(data2) - 1);
    }
    else if (greet_style == 0) {
        memcpy(buffer, data0, sizeof(data0) - 1);
    }
    else {
        buffer[0] = '\0';
    }

    strncat(inputStr, buffer, sizeof(buffer) - strlen(inputStr) - 1);
    puts(inputStr);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <first_name> <last_name>\n", argv[0]);
        return 1;
    }

    char buffer[80] = {0};

    strncpy(buffer, argv[1], 40);
    buffer[40] = '\0';

    strncpy(buffer + 40, argv[2], 32);
    buffer[72] = '\0';

    char *env = getenv("GREET_STYLE");
    if (env != NULL) {
        if (strcmp(env, "0") == 0) {
            greet_style = 1;
        } else if (strcmp(env, "1") == 0) {
            greet_style = 2;
        }
    }

    greetuser(buffer);

    return 0;
}
