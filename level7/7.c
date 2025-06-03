#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

typedef struct {
    int a;
    char *ptr;
} Block;

void m() {
    time_t t = time(NULL);
    printf((char *)0x080486e0, (char *)0x08049960, t);
}

int main(int argc, char **argv) {
    Block *block1 = malloc(8);
    block1->a = 1;
    block1->ptr = malloc(8);

    Block *block2 = malloc(8);
    block2->a = 2;
    block2->ptr = malloc(8);

    strcpy(block1->ptr, argv[1]);
    strcpy(block2->ptr, argv[2]);

    FILE *f = fopen("\x89\x86\x04\x08", "\x8b\x86\x04\x08");
    fgets((char *)0x08049960, 0x44, f);

    puts((char *)0x08048703);

    return 0;
}
