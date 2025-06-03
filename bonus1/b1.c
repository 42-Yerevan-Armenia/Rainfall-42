int main(int argc, char **argv) {
    int val = atoi(argv[1]);
    if (val > 9)
        return 1;

    memcpy((char *)&val + 5, argv[2], val * 4);

    if (val == 0x574f4c46)
        execl("/bin/sh", "sh", NULL);

    return 0;
}
