#include <stdio.h>
#include <stdlib.h>

void v() {
    char buffer[512];
    fgets(buffer, 512, *(FILE **)0x8049860);
    printf(buffer);

    if (*(int *)0x804988c == 0x40) {
        void *ptr = *(void **)0x8049880;
        fwrite((void *)0x8048600, 1, 12, ptr);
        system((char *)0x804860d);
    }
}

int main() {
    v();
    return 0;
}

