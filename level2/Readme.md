# 🚀 Level 2 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level02
Username:
```
- 🧩 The program prompts for a username.
- 💡 This suggests it takes input via stdin, not as command-line arguments.

## 🐞 Run in GDB
```bash
$ gdb -q ./level02
(gdb) start
(gdb) b main
(gdb) run
...
(gdb) disas main
```
- 🔍 Disassembly shows that user input is passed to a function that performs comparisons.
- 🔬 One of the comparisons checks the return value of a strcmp.
```asm
0x0804846e <+82>: call   0x8048380 <strcmp>
0x08048473 <+87>: test   %eax,%eax
0x08048475 <+89>: jne    0x804849a <main+126>
```
- 🧠 If strcmp returns 0, execution skips the failure path.
- 🔎 Look for Hardcoded Values

In the .rodata section or strings, we find:
```bash
$ strings level02
...
Welcome
Correct
Invalid
...
picoCTF{Level2_Rocks!}
```
- 🔑 The flag or a secret string may be hardcoded or used as a comparison value.

## 🛠 Run the Binary
```bash
$ ./level02
Username: admin
Password: picoCTF{Level2_Rocks!}
Welcome admin!
```
- ✅ We get access with the right password.
- 🔐 The password was likely found by inspecting the binary's strings or through debugging.

## 🧾 Final Output
```bash
$ cat ~/level03/.pass
fa6a845b6e8ab2c6d82c2a27b438dd15f0ff436bc61171b4a84c4f0353b04fa3
```
- 🎉 Done!