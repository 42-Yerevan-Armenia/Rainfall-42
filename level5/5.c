#include <stdio.h>
#include <stdlib.h>

void n() {
    char buffer[0x200];  // 512 bytes
    fgets(buffer, 0x200, (FILE *)0x8049848);
    printf("%s", buffer);
    exit(1);
}

void o() {
    system("/bin/sh");
    _exit(1);
}

int main() {
    n();
    return 0;
}
