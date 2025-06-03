#include <stdio.h>
#include <stdlib.h>

void p(char *input) {
    printf(input);
}

void n() {
    char buffer[520];
    fgets(buffer, 512, *(FILE **)0x8049804);
    p(buffer);
    if (*(int *)0x8049810 == 0x1025544) {
        system((char *)0x8048590);
    }
}

int main() {
    n();
    return 0;
}
