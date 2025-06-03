#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    FILE *file;
    char buffer[131] = {0};

    file = fopen("filename", "r");
    if (!file) {
        return -1;
    }

    fread(buffer, 1, 33, file);

    if (argc < 2) {
        fclose(file);
        return -1;
    }
    int index = atoi(argv[1]);
    if (index < 0 || index >= 33) {
        fclose(file);
        return -1;
    }

    buffer[index] = '\0';

    fread(buffer + 66, 1, 65, file);
    fclose(file);

    if (strcmp(argv[1], buffer) == 0) {
        execl("/bin/sh", "sh", NULL);
    } else {
        puts(buffer + 66);
    }

    return 0;
}
