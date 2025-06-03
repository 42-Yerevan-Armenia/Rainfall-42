#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void p() {
    char buffer[0x44];
    fflush(stdout);
    gets(buffer);
    void *saved_ebp = __builtin_frame_address(0);
    void *ret_addr = *((void **)saved_ebp + 1);
    if (((unsigned int)ret_addr & 0xB0000000) == 0xB0000000) {
        printf("You are trying to access a forbidden address: %p\n", ret_addr);
        _exit(1);
    }
    puts(buffer);
    strdup(buffer);
}

int main() {
    p();
    return 0;
}

