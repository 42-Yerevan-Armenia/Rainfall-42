# 🚀 Level 8 - Reverse Engineering
## 🧪 First Step – Testing the Program
When running the program without arguments:

```bash
$ ./level8
(nil), (nil)

(nil), (nil)
coucou
(nil), (nil)
c
(nil), (nil)
aaaa
(nil), (nil)
^C
```
With an argument:

```bash
$ ./level8 coucou
(nil), (nil)

(nil), (nil)
^C
```
Nothing useful is printed beyond two (nil) pointers, suggesting there's some state not yet triggered.

## 🐞 Static Analysis – main() Function in GDB
The disassembly gives us insight into the logic.

### 🔍 Summary of Key Observations
- The program repeatedly prints two values from addresses:

```c
0x8049ab0  ; <- potentially a global pointer
0x8049aac  ; <- another global pointer
```
These are printed with:

```c
printf("%p, %p\n", *ptr1, *ptr2);
```
- Then it enters a loop:

```c
fgets(input_buffer, 0x80, stdin);
```

## 🔐 Conditional String Checks
The input is compared three times using strncmp-like logic (repz cmps):

1. Compare input with "coucou"
```asm
0x080485c1: mov eax, 0x8048819  ; "coucou"
...
```
cmp input, "coucou"
If equal:
- Allocates 4 bytes with malloc(4)
- Sets the pointer at 0x8049aac to it
- Copies the rest of the input after "coucou" into that pointer (max 0x1e bytes)

2. Compare input with "test"
```asm
0x08048648: mov eax, 0x804881f  ; "test"
```
If equal:
- Frees the malloc'd buffer pointed to by 0x8049aac

3. Compare input with "exploit"
```asm
0x0804867e: mov eax, 0x8048825  ; "exploit"
```
If equal:
- Calls strdup(input + 7)
- Stores pointer at 0x8049ab0

## 📌 Vulnerability Summary
- Use-After-Free potential:
If strdup is called after free, the pointer 0x8049aac may point to reclaimed memory.
- Pointer State:
- - `"coucou..."` → allocates memory and stores content
- - `"test"` → frees pointer
- - `"exploit..."` → duplicates rest of string and stores
- Goal: Overwrite a pointer or trigger code via malicious input and memory manipulation.

## 🧪 Exploit Strategy Idea
#### Step 1: Input a payload like:

```
coucouAAAA
```
→ Allocates memory and writes `AAAA` into it.

#### Step 2: Input:

```bash
test
```
→ Frees the pointer.
#### Step 3: Input:
```
exploitBBBB...
```
→ Duplicates and sets a pointer, possibly reusing freed memory.

## 🗒️ Tips for Further Analysis
- Check the addresses at 0x8049aac and 0x8049ab0 at each step in GDB.
- Dump heap and look for reallocation overlap.
- Use set follow-fork-mode child if the program forks.

## 📚 Keywords and Functions
| Instruction | Meaning |
|:-----:|:-------------:|
| fgets	| Read user input |
| malloc	| Allocate memory |
| free	| Free allocated memory |
| strdup	| Duplicate a string |
| strcmp	| Compare strings (repz cmps) |

## 📦 Conclusion
The program manages two global pointers and responds to specific inputs. By feeding it "coucou", "test", and "exploit" inputs in a crafted sequence, it may be possible to exploit a Use-After-Free or a memory corruption vulnerability.

Stay tuned for the next steps: memory layout analysis, pointer tracking, and eventually, RCE or flag leak.

