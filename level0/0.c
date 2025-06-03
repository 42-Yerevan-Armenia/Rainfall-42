#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (atoi(argv[1]) == 423) {
        char *path = strdup("/some/constant/string");
        gid_t gid = getegid();
        uid_t uid = geteuid();
        setresgid(gid, gid, gid);
        setresuid(uid, uid, uid);
        execv(path, &path);
    } else {
        fwrite((void *)0x80c5350, 1, 5, *(FILE **)0x80ee170);
    }
    return 0;
}

