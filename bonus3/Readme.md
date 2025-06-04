# 🎯 bonus3 - Walkthrough

## 🧪 First Step - Testing the Program
```bash
$ ./bonus3
$ ./bonus3 coucou
$ ./bonus3 coucou coucou
```
Not very interesting at first glance.

## 🐞 Start in GDB
```bash
gdb-peda$ pdisas main
```
- A lot of operations happen, including:
- - Opening and reading the file /home/user/end/.pass
- - Using atoi and strcmp on program arguments
- - Calling execl("/bin/sh", "sh", 0) under certain conditions

## 🔎 Analysis of Key Logic
```c
if (strcmp(buffer, av[1]) == 0)
```
- This condition must be true to execute execl.
Just before, the code sets a null byte inside buffer based on the integer value of av[1]:

```c
nb = atoi(av[1]);
buffer[nb] = 0;
```
- To get buffer[0] = '\0', nb must be 0, meaning av[1] should be "" (empty string).

## ✅ Final Step - Run with Empty Argument
```bash
$ ./bonus3 ''
$
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```
- 🎉 Done!
