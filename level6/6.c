#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void n() {
    system((char *)0x080485b0);
}

void m() {
    puts((char *)0x080485d1);
}

int main(int argc, char **argv) {
    void *buf;
    void (**func)();

    buf = malloc(0x40);
    func = malloc(sizeof(void (*)(void)));

    *func = m;

    if (argc > 1) {
        strcpy(buf, argv[1]);
        (*func)();
    }

    return 0;
}
