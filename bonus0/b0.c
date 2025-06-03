#include <stdio.h>
#include <string.h>

void p(char *dest) {
    puts(dest);

    char buffer[0x1000];

    read(0, buffer, 0x1000);

    char *newline = strchr(buffer, '\n');
    if (newline) {
        *newline = '\0';
    }

    strncpy(dest, buffer, 20);
}


void pp(char *str) {
    char buf1[48];
    char buf2[28];

    p(buf1);
    p(buf2);

    strcpy(str, buf1);

    int len = strlen(str);
    unsigned short *data = (unsigned short *)0x80486a4;
    *(unsigned short *)(str + len) = *data;

    strcat(str, buf2);
}

int main() {
    char buffer[64];

    pp(buffer + 22);
    puts(buffer + 22);

    return 0;
}
