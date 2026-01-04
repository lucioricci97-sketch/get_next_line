# GET_NEXT_LINE - VISUAL FLOW DIAGRAMS

## 📊 COMPLETE EXECUTION FLOW

### Example File: "Hello World\n42 School\n"

```
┌─────────────────────────────────────────────────────────────────┐
│                         FILE CONTENT                              │
│                   "Hello World\n42 School\n"                      │
└─────────────────────────────────────────────────────────────────┘

═══════════════════════════════════════════════════════════════════
                      FIRST CALL: get_next_line(fd)
═══════════════════════════════════════════════════════════════════

STEP 1: Initialize
┌─────────────────┐
│ static buffer = NULL                                              │
│ char *line                                                        │
└─────────────────┘

STEP 2: Validation
┌─────────────────┐
│ if (fd < 0 || BUFFER_SIZE <= 0)  ← Check fd=3, BUFFER_SIZE=5    │
│     return (NULL);                 ← PASS ✓                       │
└─────────────────┘

STEP 3: read_file(fd, NULL)  [BUFFER_SIZE = 5]
┌─────────────────────────────────────────────────────────────────┐
│ res = NULL                                                        │
│ buffer = malloc(6) = "\0\0\0\0\0\0"                              │
│                                                                   │
│ LOOP 1:                                                           │
│   ├─ ft_strchr(NULL, '\n') = NULL  → Continue                   │
│   ├─ read(fd, buffer, 5) = 5                                    │
│   ├─ buffer = "Hello"                                            │
│   ├─ buffer[5] = '\0'  →  buffer = "Hello\0"                    │
│   └─ res = ft_strjoin(NULL, "Hello") = "Hello"                  │
│        └─ Allocates 6 bytes: "Hello\0"                           │
│                                                                   │
│ LOOP 2:                                                           │
│   ├─ ft_strchr("Hello", '\n') = NULL  → Continue                │
│   ├─ read(fd, buffer, 5) = 5                                    │
│   ├─ buffer = " Worl"                                            │
│   ├─ buffer[5] = '\0'  →  buffer = " Worl\0"                    │
│   └─ res = ft_strjoin("Hello", " Worl") = "Hello Worl"          │
│        ├─ Allocates 11 bytes: "Hello Worl\0"                     │
│        └─ Frees old "Hello"                                       │
│                                                                   │
│ LOOP 3:                                                           │
│   ├─ ft_strchr("Hello Worl", '\n') = NULL  → Continue           │
│   ├─ read(fd, buffer, 5) = 2                                    │
│   ├─ buffer = "d\n"                                              │
│   ├─ buffer[2] = '\0'  →  buffer = "d\n\0"                      │
│   └─ res = ft_strjoin("Hello Worl", "d\n") = "Hello World\n"    │
│        ├─ Allocates 13 bytes: "Hello World\n\0"                  │
│        └─ Frees old "Hello Worl"                                 │
│                                                                   │
│ LOOP EXIT:                                                        │
│   └─ ft_strchr("Hello World\n", '\n') = found!  → Stop          │
│                                                                   │
│ free(buffer)                                                      │
│ return "Hello World\n"                                            │
└─────────────────────────────────────────────────────────────────┘

STEP 4: buffer = "Hello World\n"

STEP 5: ft_line("Hello World\n")
┌─────────────────────────────────────────────────────────────────┐
│ Count len:                                                        │
│   len = 0                                                         │
│   buffer[0]='H' != '\n'  → len=1                                │
│   buffer[1]='e' != '\n'  → len=2                                │
│   ...                                                             │
│   buffer[10]='d' != '\n' → len=11                               │
│   buffer[11]='\n'        → STOP                                  │
│   len = 11                                                        │
│                                                                   │
│ Allocate:                                                         │
│   line = calloc(11 + 2, 1) = 13 bytes                           │
│   line = "\0\0\0\0\0\0\0\0\0\0\0\0\0"                           │
│                                                                   │
│ Copy:                                                             │
│   j=0: line[0]='H'   j=1: line[1]='e'   j=2: line[2]='l'       │
│   j=3: line[3]='l'   j=4: line[4]='o'   j=5: line[5]=' '       │
│   j=6: line[6]='W'   j=7: line[7]='o'   j=8: line[8]='r'       │
│   j=9: line[9]='l'   j=10: line[10]='d'                         │
│   buffer[11]='\n' → line[11]='\n', j=12                         │
│                                                                   │
│ Null terminate:                                                   │
│   line[12] = '\0'                                                │
│                                                                   │
│ return "Hello World\n"                                            │
└─────────────────────────────────────────────────────────────────┘

STEP 6: line = "Hello World\n"

STEP 7: ft_next("Hello World\n")
┌─────────────────────────────────────────────────────────────────┐
│ Find '\n' position:                                               │
│   len = 0                                                         │
│   buffer[0]='H' != '\n'  → len=1                                │
│   ...                                                             │
│   buffer[11]='\n'        → STOP                                  │
│   len = 11                                                        │
│                                                                   │
│ Check if '\n' exists:                                             │
│   buffer[11] = '\n'  (not '\0')  → Continue                     │
│                                                                   │
│ Calculate size:                                                   │
│   strlen("Hello World\n") = 12                                    │
│   size = 12 - 11 + 1 = 2  (but we skip '\n', so actually 1)    │
│                                                                   │
│ Skip '\n':                                                        │
│   len++ → len = 12                                               │
│                                                                   │
│ Copy remainder:                                                   │
│   buffer[12] = '\0'  → Nothing left!                            │
│   new_buffer = "\0"  (empty)                                     │
│                                                                   │
│ Wait, file has more!                                              │
│ Actually buffer ONLY had "Hello World\n" so far                   │
│ We'll read "42 School\n" next time                               │
│                                                                   │
│ free(old buffer)                                                  │
│ return NULL  (nothing left in THIS buffer)                        │
└─────────────────────────────────────────────────────────────────┘

STEP 8: buffer = NULL

STEP 9: return "Hello World\n"

═══════════════════════════════════════════════════════════════════

RESULT OF FIRST CALL:
┌─────────────────┐
│ Returned to caller: "Hello World\n"  (user must free this)      │
│ Static buffer: NULL  (will read more next time)                  │
│ File position: After "Hello World\n" (at '4' of "42 School\n")  │
└─────────────────┘

═══════════════════════════════════════════════════════════════════
                      SECOND CALL: get_next_line(fd)
═══════════════════════════════════════════════════════════════════

STEP 1: Initialize
┌─────────────────┐
│ static buffer = NULL  (from previous call)                        │
│ char *line                                                        │
└─────────────────┘

STEP 2: Validation [PASS]

STEP 3: read_file(fd, NULL)  [BUFFER_SIZE = 5]
┌─────────────────────────────────────────────────────────────────┐
│ res = NULL                                                        │
│ buffer = malloc(6)                                                │
│                                                                   │
│ LOOP 1:                                                           │
│   ├─ read(fd, buffer, 5) = 5                                    │
│   ├─ buffer = "42 Sc"                                            │
│   └─ res = ft_strjoin(NULL, "42 Sc") = "42 Sc"                  │
│                                                                   │
│ LOOP 2:                                                           │
│   ├─ ft_strchr("42 Sc", '\n') = NULL                            │
│   ├─ read(fd, buffer, 5) = 5                                    │
│   ├─ buffer = "hool\n"                                           │
│   └─ res = ft_strjoin("42 Sc", "hool\n") = "42 School\n"        │
│                                                                   │
│ LOOP EXIT:                                                        │
│   └─ ft_strchr("42 School\n", '\n') = found!                    │
│                                                                   │
│ return "42 School\n"                                              │
└─────────────────────────────────────────────────────────────────┘

STEP 4: buffer = "42 School\n"

STEP 5: ft_line("42 School\n") → "42 School\n"

STEP 6: ft_next("42 School\n") → NULL  (nothing after '\n')

STEP 7: buffer = NULL

STEP 8: return "42 School\n"

═══════════════════════════════════════════════════════════════════

RESULT OF SECOND CALL:
┌─────────────────┐
│ Returned: "42 School\n"                                          │
│ Static buffer: NULL                                               │
│ File position: EOF (end of file)                                  │
└─────────────────┘

═══════════════════════════════════════════════════════════════════
                      THIRD CALL: get_next_line(fd)
═══════════════════════════════════════════════════════════════════

STEP 1: buffer = NULL

STEP 2: Validation [PASS]

STEP 3: read_file(fd, NULL)
┌─────────────────────────────────────────────────────────────────┐
│ LOOP 1:                                                           │
│   ├─ read(fd, buffer, 5) = 0  (EOF!)                            │
│   └─ byte_read = 0  → Exit loop                                  │
│                                                                   │
│ return NULL  (res is still NULL)                                 │
└─────────────────────────────────────────────────────────────────┘

STEP 4: buffer = NULL

STEP 5: if (!buffer) → return NULL

═══════════════════════════════════════════════════════════════════

RESULT OF THIRD CALL:
┌─────────────────┐
│ Returned: NULL  (end of file)                                     │
└─────────────────┘
```

---

## 🔄 MEMORY STATE DIAGRAM

### Through Multiple Calls

```
═══════════════════════════════════════════════════════════════════
                           CALL 1
═══════════════════════════════════════════════════════════════════

BEFORE:
┌──────────────────┐
│ static buffer    │ = NULL
└──────────────────┘

DURING read_file:
┌──────────────────┐
│ temp buffer      │ = "Hello" → "World\n" (freed at end)
│ res              │ = "Hello" → "Hello World\n"
└──────────────────┘

AFTER read_file:
┌──────────────────┐
│ static buffer    │ = "Hello World\n"
└──────────────────┘

AFTER ft_line:
┌──────────────────┐
│ line (returned)  │ = "Hello World\n"  ← User must free!
│ static buffer    │ = "Hello World\n"  (unchanged)
└──────────────────┘

AFTER ft_next:
┌──────────────────┐
│ line (returned)  │ = "Hello World\n"  ← User must free!
│ static buffer    │ = NULL  (freed old, nothing left)
└──────────────────┘

═══════════════════════════════════════════════════════════════════
                           CALL 2
═══════════════════════════════════════════════════════════════════

BEFORE:
┌──────────────────┐
│ static buffer    │ = NULL (from call 1)
└──────────────────┘

DURING read_file:
┌──────────────────┐
│ temp buffer      │ = "42 Sc" → "hool\n" (freed)
│ res              │ = "42 School\n"
└──────────────────┘

AFTER:
┌──────────────────┐
│ line (returned)  │ = "42 School\n"  ← User must free!
│ static buffer    │ = NULL
└──────────────────┘

═══════════════════════════════════════════════════════════════════
                           CALL 3
═══════════════════════════════════════════════════════════════════

┌──────────────────┐
│ read() = 0       │ → EOF
│ return NULL      │
└──────────────────┘
```

---

## 🎁 BONUS: MULTIPLE FILE DESCRIPTORS

### Example: Reading 3 Files Simultaneously

```
Files:
  file1.txt (fd=3): "AAA\nBBB\n"
  file2.txt (fd=4): "XXX\nYYY\n"
  file3.txt (fd=5): "111\n222\n"

═══════════════════════════════════════════════════════════════════
                           INITIAL STATE
═══════════════════════════════════════════════════════════════════

static char *stash[1024] = {
  [0] = NULL,
  [1] = NULL,
  [2] = NULL,
  [3] = NULL,  ← Will be used for fd=3 (file1)
  [4] = NULL,  ← Will be used for fd=4 (file2)
  [5] = NULL,  ← Will be used for fd=5 (file3)
  ...
  [1023] = NULL
};

═══════════════════════════════════════════════════════════════════
                    get_next_line(3) - FIRST CALL
═══════════════════════════════════════════════════════════════════

stash[3] = read_file(3, NULL)     → "AAA\nBBB\n"
line = ft_line(stash[3])          → "AAA\n"
stash[3] = ft_next(stash[3])      → "BBB\n"
return "AAA\n"

STATE AFTER:
┌─────────────────────────────────────────────────────────────────┐
│ stash[3] = "BBB\n"  ← Saved for file1                            │
│ stash[4] = NULL     ← Not touched                                 │
│ stash[5] = NULL     ← Not touched                                 │
└─────────────────────────────────────────────────────────────────┘

═══════════════════════════════════════════════════════════════════
                    get_next_line(4) - FIRST CALL
═══════════════════════════════════════════════════════════════════

stash[4] = read_file(4, NULL)     → "XXX\nYYY\n"
line = ft_line(stash[4])          → "XXX\n"
stash[4] = ft_next(stash[4])      → "YYY\n"
return "XXX\n"

STATE AFTER:
┌─────────────────────────────────────────────────────────────────┐
│ stash[3] = "BBB\n"  ← Still there! Not affected!                 │
│ stash[4] = "YYY\n"  ← Saved for file2                            │
│ stash[5] = NULL     ← Not touched                                 │
└─────────────────────────────────────────────────────────────────┘

═══════════════════════════════════════════════════════════════════
                    get_next_line(5) - FIRST CALL
═══════════════════════════════════════════════════════════════════

stash[5] = read_file(5, NULL)     → "111\n222\n"
line = ft_line(stash[5])          → "111\n"
stash[5] = ft_next(stash[5])      → "222\n"
return "111\n"

STATE AFTER:
┌─────────────────────────────────────────────────────────────────┐
│ stash[3] = "BBB\n"  ← Still there!                               │
│ stash[4] = "YYY\n"  ← Still there!                               │
│ stash[5] = "222\n"  ← Saved for file3                            │
└─────────────────────────────────────────────────────────────────┘

═══════════════════════════════════════════════════════════════════
                 get_next_line(3) - SECOND CALL (back to file1!)
═══════════════════════════════════════════════════════════════════

stash[3] = "BBB\n"  ← Retrieved!
stash[3] = read_file(3, "BBB\n")  → Already has '\n', doesn't read
line = ft_line("BBB\n")           → "BBB\n"
stash[3] = ft_next("BBB\n")       → NULL (nothing left)
return "BBB\n"

STATE AFTER:
┌─────────────────────────────────────────────────────────────────┐
│ stash[3] = NULL     ← Used up                                     │
│ stash[4] = "YYY\n"  ← Still there! Independent!                  │
│ stash[5] = "222\n"  ← Still there! Independent!                  │
└─────────────────────────────────────────────────────────────────┘

═══════════════════════════════════════════════════════════════════
                         VISUAL SUMMARY
═══════════════════════════════════════════════════════════════════

Time: ──────────────────────────────────────────────>

Calls:  GNL(3)   GNL(4)   GNL(5)   GNL(3)   GNL(4)   GNL(5)
         │        │        │        │        │        │
fd=3:   AAA\n    │        │       BBB\n     │        │
         │        │        │        │        │        │
fd=4:    │      XXX\n     │        │      YYY\n     │
         │        │        │        │        │        │
fd=5:    │        │      111\n     │        │      222\n

Each fd maintains its own independent state!
```

---

## 🔍 EDGE CASES VISUALIZATION

### Edge Case 1: No Newline at EOF

```
File: "Hello"  (no '\n' at end)

CALL 1:
┌─────────────────────────────────────────────────────────────────┐
│ read_file:                                                        │
│   read(fd, buffer, 5) → "Hello" (5 bytes)                       │
│   res = "Hello"                                                   │
│   ft_strchr("Hello", '\n') → NULL (no '\n')                     │
│   read(fd, buffer, 5) → 0 (EOF)                                 │
│   Loop exits (byte_read = 0)                                     │
│   return "Hello"                                                  │
│                                                                   │
│ ft_line("Hello"):                                                 │
│   len = 5 (no '\n' found)                                        │
│   Allocate 7 bytes                                                │
│   Copy: "Hello"                                                   │
│   No '\n' to add                                                  │
│   return "Hello"  (WITHOUT '\n')                                 │
│                                                                   │
│ ft_next("Hello"):                                                 │
│   len = 5                                                         │
│   buffer[5] = '\0' (no '\n')                                     │
│   !buffer[len] → TRUE                                             │
│   free(buffer)                                                    │
│   return NULL                                                     │
└─────────────────────────────────────────────────────────────────┘

Result: "Hello" (no newline)

CALL 2:
┌─────────────────────────────────────────────────────────────────┐
│ buffer = NULL                                                     │
│ read_file → read() returns 0 → return NULL                      │
│ return NULL                                                       │
└─────────────────────────────────────────────────────────────────┘

Result: NULL (EOF)
```

### Edge Case 2: Empty File

```
File: ""  (empty, 0 bytes)

CALL 1:
┌─────────────────────────────────────────────────────────────────┐
│ read_file:                                                        │
│   res = NULL                                                      │
│   read(fd, buffer, 5) → 0 (EOF immediately)                     │
│   Loop exits (byte_read = 0)                                     │
│   return NULL                                                     │
│                                                                   │
│ if (!buffer) → TRUE                                               │
│ return NULL                                                       │
└─────────────────────────────────────────────────────────────────┘

Result: NULL
```

### Edge Case 3: Very Long Line (1000 chars, BUFFER_SIZE=5)

```
File: "A" × 1000 + "\n"

CALL 1:
┌─────────────────────────────────────────────────────────────────┐
│ read_file:                                                        │
│   Loop 1: read 5 → res = "AAAAA"                                │
│   Loop 2: read 5 → res = "AAAAAAAAAA"                           │
│   Loop 3: read 5 → res = "AAAAAAAAAAAAAAA"                      │
│   ...                                                             │
│   Loop 200: read 5 → res = "AAA...AAA\n"  (1001 chars)          │
│   ft_strchr finds '\n' → Stop                                    │
│   return "AAA...AAA\n"  (1001 characters)                        │
│                                                                   │
│ ft_line: Extract all 1000 'A's + '\n'                            │
│ return "AAA...AAA\n"  (1001 bytes)                               │
└─────────────────────────────────────────────────────────────────┘

Result: Very long line (no problem!)
```

### Edge Case 4: BUFFER_SIZE = 1

```
File: "Hi\n"

CALL 1:
┌─────────────────────────────────────────────────────────────────┐
│ read_file (BUFFER_SIZE=1):                                        │
│   Loop 1: read(fd, buffer, 1) → "H"                             │
│           res = "H"                                               │
│           ft_strchr("H", '\n') → NULL                            │
│                                                                   │
│   Loop 2: read(fd, buffer, 1) → "i"                             │
│           res = ft_strjoin("H", "i") = "Hi"                      │
│           ft_strchr("Hi", '\n') → NULL                           │
│                                                                   │
│   Loop 3: read(fd, buffer, 1) → "\n"                            │
│           res = ft_strjoin("Hi", "\n") = "Hi\n"                  │
│           ft_strchr("Hi\n", '\n') → FOUND!                       │
│   Exit loop                                                       │
│   return "Hi\n"                                                   │
└─────────────────────────────────────────────────────────────────┘

Result: "Hi\n" (works perfectly, just slower!)
```

---

## 💡 KEY INSIGHTS

### 1. Static Variable Behavior

```
┌──────────────────────────────────────────────────────────────┐
│  PROGRAM START                                                │
├──────────────────────────────────────────────────────────────┤
│  static char *buffer = NULL;  ← Initialized ONCE              │
└──────────────────────────────────────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────────┐
│  CALL 1: get_next_line()                                      │
│    buffer = "remaining data"  ← Modified                      │
└──────────────────────────────────────────────────────────────┘
                           │
                           ▼ (buffer keeps value)
┌──────────────────────────────────────────────────────────────┐
│  CALL 2: get_next_line()                                      │
│    buffer still = "remaining data"  ← Value retained!         │
└──────────────────────────────────────────────────────────────┘
                           │
                           ▼ (buffer keeps value)
┌──────────────────────────────────────────────────────────────┐
│  CALL 3: get_next_line()                                      │
│    buffer still remembers previous value!                     │
└──────────────────────────────────────────────────────────────┘
                           │
                           ▼
┌──────────────────────────────────────────────────────────────┐
│  PROGRAM END                                                   │
│    buffer is destroyed  ← Only when program exits             │
└──────────────────────────────────────────────────────────────┘
```

### 2. Memory Ownership Chain

```
read_file() allocates → res
                         │
                         ▼
ft_strjoin() frees old, allocates new → res
                                         │
                                         ▼
                              Returned as buffer
                                         │
                                         ▼
ft_line() allocates → line (GIVEN TO CALLER)
                       │
                       └→ CALLER MUST FREE!

ft_next() frees old buffer, allocates new → buffer (KEPT IN STATIC)
```

### 3. Why The Design Works

```
┌────────────────────────────────────────────────────────────────┐
│  Separation of Concerns:                                        │
│                                                                  │
│  read_file()  →  "Get enough data"                              │
│                  Knows about: reading, '\n' checking             │
│                  Doesn't know about: line extraction             │
│                                                                  │
│  ft_line()    →  "Extract one line"                             │
│                  Knows about: finding '\n', copying              │
│                  Doesn't know about: file reading                │
│                                                                  │
│  ft_next()    →  "Prepare for next time"                        │
│                  Knows about: remainder after '\n'               │
│                  Doesn't know about: how line was extracted      │
│                                                                  │
│  Result: Each function is simple, testable, debuggable!         │
└────────────────────────────────────────────────────────────────┘
```

---

## 🎯 QUICK REFERENCE

### Common Patterns

**Pattern 1: Read until condition**
```c
while (!found_newline && !eof)
    read_more();
```

**Pattern 2: Allocate-Copy-Free**
```c
new = malloc(size);
copy_data(old, new);
free(old);
return new;
```

**Pattern 3: String without newline**
```c
len = count_until_newline();  // Don't count '\n'
alloc(len + 2);               // +1 for '\n', +1 for '\0'
copy_with_newline();          // Copy including '\n'
```

### Common Mistakes to Avoid

❌ `malloc(len + 1)` for line  → Missing space for '\n'  
✅ `malloc(len + 2)`            → Correct: line + '\n' + '\0'

❌ `while (buffer[len] != '\n')` → Segfault if no '\n'  
✅ `while (buffer[len] && buffer[len] != '\n')` → Safe

❌ Forget to null-terminate after read()  
✅ `buffer[byte_read] = '\0'` → Always do this!

❌ Don't free before returning NULL on error  
✅ Free all allocated memory before error return

---

**This guide should give you complete confidence for your evaluation! 🚀**
