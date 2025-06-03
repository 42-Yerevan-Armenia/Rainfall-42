# 🚀 Level 0 - Walkthrough

## 🧪 First Step - Testing the Program

```bash
$ ./level0
Segmentation fault (core dumped)

$ ./level0 coucou
No !
```
- ❗ The program segfaults without parameters and prints "No !" when given a parameter.
- 💡 We can assume the program expects a password as a parameter.

## 🐞 Start the Program in GDB
```
$ gdb level0 42
(gdb) disas main
...
   0x08048ed4 <+20>:	call   0x8049710 <atoi>
   0x08048ed9 <+25>:	cmp    $0x1a7,%eax
...
```
- 🔍 The function atoi is called, and the result is compared to 0x1a7 (423 in decimal).

## 🔐 Try This Number
```bash
$ ./level0 423
$ cat ~/level1/.pass
```

## ✅ Success! We have the shell:
```
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```
