# GET_NEXT_LINE - COMPLETE EXPLANATION GUIDE
## For 42 School Evaluation

---

## 📚 TABLE OF CONTENTS

1. [Fundamental Concepts](#fundamental-concepts)
2. [Program Overview](#program-overview)
3. [Mandatory Part - Line by Line](#mandatory-part)
4. [Bonus Part - Multiple File Descriptors](#bonus-part)
5. [Design Decisions](#design-decisions)
6. [Common Evaluator Questions](#common-questions)

---

## 🎯 FUNDAMENTAL CONCEPTS

### What is a File Descriptor (fd)?

**Simple Definition:**
A file descriptor is a **number** (integer) that represents an open file in your program. Think of it like a **ticket number** at a restaurant - you give them your number, and they know which order is yours.

**Technical Definition:**
A file descriptor is a non-negative integer that acts as an abstract handle to access I/O resources (files, pipes, sockets, etc.). The operating system kernel maintains a **file descriptor table** for each process.

**Standard File Descriptors:**
```c
0 = STDIN  (Standard Input  - keyboard input)
1 = STDOUT (Standard Output - terminal output)
2 = STDERR (Standard Error  - error messages)
```

**How it works in practice:**
```c
int fd;
fd = open("file.txt", O_RDONLY);  // Returns 3 (first available fd after 0,1,2)
// Now fd = 3 is your "ticket" to access file.txt
// When you call read(fd, ...), the OS knows you mean file.txt
```

**Important facts:**
- File descriptors are **per-process** (each program has its own set)
- Valid fd values: 0 to 1023 (on most systems, can be higher)
- Negative fd values are **invalid** and indicate errors
- After `close(fd)`, that number can be reused

---

### The `open()` System Call

**Prototype:**
```c
#include <fcntl.h>
int open(const char *pathname, int flags);
```

**What it does:**
Opens a file and returns a file descriptor that you can use to read/write.

**Parameters:**
1. `pathname` - Path to the file (e.g., "file.txt", "/path/to/file")
2. `flags` - How you want to open it:
   - `O_RDONLY` - Read only (0)
   - `O_WRONLY` - Write only (1)
   - `O_RDWR`   - Read and write (2)

**Return value:**
- **Success:** Returns file descriptor (integer ≥ 0)
- **Failure:** Returns -1 (file doesn't exist, no permission, etc.)

**Example:**
```c
int fd = open("test.txt", O_RDONLY);
if (fd == -1)
    printf("Error: Could not open file\n");
else
    printf("File opened successfully with fd = %d\n", fd);
```

---

### The `read()` System Call

**Prototype:**
```c
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
```

**What it does:**
Reads data from an open file descriptor into a buffer.

**Parameters:**
1. `fd` - File descriptor (from `open()`)
2. `buf` - Buffer (array) where data will be stored
3. `count` - Maximum number of bytes to read

**Return value:**
- **> 0:** Number of bytes actually read
- **0:** End of file (EOF) reached
- **-1:** Error occurred (invalid fd, read error, etc.)

**Important behavior:**
- `read()` does NOT automatically add '\0' at the end
- It reads UP TO `count` bytes (might read less)
- Each call advances the file position (cursor)
- When you reach EOF, it returns 0

**Example:**
```c
char buffer[10];
int fd = open("file.txt", O_RDONLY);
int bytes = read(fd, buffer, 10);

// If file contains "Hello"
// bytes = 5
// buffer = "Hello?????" (no automatic null terminator!)
// You must do: buffer[bytes] = '\0';
```

**Visual example of multiple reads:**
```
File content: "Hello World!\n"
              ↑ cursor starts here

read(fd, buf, 5)  → Returns 5, buf = "Hello", cursor moves to 'W'
read(fd, buf, 5)  → Returns 5, buf = " Worl", cursor moves to 'd'
read(fd, buf, 5)  → Returns 3, buf = "d!\n",  cursor at EOF
read(fd, buf, 5)  → Returns 0 (EOF reached)
```

---

### Static Variables

**Normal Variables:**
```c
void function() {
    int x = 0;  // Created when function is called
    x++;
    printf("%d\n", x);  // Always prints 1
}  // x is DESTROYED here
```

**Static Variables:**
```c
void function() {
    static int x = 0;  // Created ONCE, keeps value between calls
    x++;
    printf("%d\n", x);
}  // x is NOT destroyed

// First call:  prints 1 (x becomes 1)
// Second call: prints 2 (x becomes 2)
// Third call:  prints 3 (x becomes 3)
```

**Key differences:**

| Feature | Normal Variable | Static Variable |
|---------|----------------|-----------------|
| **Lifetime** | Function call only | Entire program |
| **Initialization** | Every call | Only first time |
| **Memory location** | Stack | Data segment |
| **Value retention** | No | Yes |

**Why we use static in get_next_line:**

We need to **remember** what we read from the file between function calls!

```c
// Call 1: Read "Hello World\nNext line\n"
//         Return "Hello World\n"
//         SAVE "Next line\n" for next time

// Call 2: Use SAVED "Next line\n"
//         Return "Next line\n"
```

**Without static:**
```c
char *get_next_line(int fd) {
    char *buffer = NULL;  // Always starts as NULL!
    // Lost previous data - can't continue reading!
}
```

**With static:**
```c
char *get_next_line(int fd) {
    static char *buffer = NULL;  // Keeps data between calls!
    // Can continue from where we left off
}
```

---

## 📖 PROGRAM OVERVIEW

### What does get_next_line do?

**Simple explanation:**
Reads and returns **one line at a time** from a file, no matter how the file was written.

**Why is it useful?**
- You don't need to know file size in advance
- Memory efficient (don't load entire file)
- Can handle huge files (GB) without problems
- Works with any line length

**Behavior:**
```c
// File: "Line1\nLine2\nLine3"

char *line1 = get_next_line(fd);  // "Line1\n"
char *line2 = get_next_line(fd);  // "Line2\n"
char *line3 = get_next_line(fd);  // "Line3"
char *line4 = get_next_line(fd);  // NULL (end of file)
```

### The BUFFER_SIZE Concept

**What is it?**
A compile-time constant that defines how many bytes we read at once.

**Why use it?**
- Reading byte-by-byte is SLOW (many system calls)
- Reading huge chunks wastes memory
- BUFFER_SIZE is a compromise

**Compilation:**
```bash
gcc -D BUFFER_SIZE=42 get_next_line.c ...
# Now BUFFER_SIZE = 42 in your program
```

**How it affects the program:**
```c
// BUFFER_SIZE = 5
// File: "Hello World\n"

read(fd, buffer, 5)  → "Hello"
read(fd, buffer, 5)  → " Worl"
read(fd, buffer, 5)  → "d\n"
// Returns "Hello World\n"

// BUFFER_SIZE = 100
// File: "Hello World\n"

read(fd, buffer, 100) → "Hello World\n" (all at once!)
// Returns "Hello World\n"
```

**Important:** The line returned can be **any length**, regardless of BUFFER_SIZE!

---

## 🔧 MANDATORY PART - LINE BY LINE

### Main Function: `get_next_line(int fd)`

**Purpose:** Entry point that orchestrates everything

```c
char *get_next_line(int fd)
{
    static char *buffer = NULL;  // [1]
    char        *line;           // [2]

    if (fd < 0 || BUFFER_SIZE <= 0)  // [3]
        return (NULL);
    
    buffer = read_file(fd, buffer);   // [4]
    if (!buffer)                      // [5]
        return (NULL);
    
    line = ft_line(buffer);           // [6]
    buffer = ft_next(buffer);         // [7]
    
    return (line);                    // [8]
}
```

**Explanation line by line:**

**[1] `static char *buffer = NULL;`**
- **What:** Pointer to store leftover data between calls
- **Why static:** Keeps value between function calls
- **Initialized to NULL:** Only on first call
- **Example:** After reading "Hello\nWorld", if we return "Hello\n", buffer stores "World"

**[2] `char *line;`**
- **What:** Will store the line we're going to return
- **Not static:** We return this, so it's temporary

**[3] `if (fd < 0 || BUFFER_SIZE <= 0)`**
- **What:** Input validation
- **fd < 0:** Invalid file descriptor (negative numbers are errors)
- **BUFFER_SIZE <= 0:** Cannot read 0 or negative bytes
- **Why:** Prevents crashes and undefined behavior
- **Alternative approaches:** Could check `fd >= 1024`, but not required for mandatory

**[4] `buffer = read_file(fd, buffer);`**
- **What:** Read from file until we have at least one complete line
- **Why pass buffer:** Continue from where we left off (might have "Wor", need to add "ld\n")
- **Return value:** Updated buffer with enough data (contains at least one '\n', or EOF)

**[5] `if (!buffer)`**
- **What:** Check if read_file failed or EOF reached
- **When NULL:** Read error, empty file, or end of file with no data left
- **Why return NULL:** Standard way to signal "no more lines"

**[6] `line = ft_line(buffer);`**
- **What:** Extract one line from buffer (everything up to and including '\n')
- **Example:** buffer = "Hello\nWorld" → line = "Hello\n"
- **Allocates memory:** We return this to the user, they must free() it

**[7] `buffer = ft_next(buffer);`**
- **What:** Update buffer to remove the line we just extracted
- **Example:** buffer was "Hello\nWorld" → now "World"
- **Frees old buffer:** Memory management
- **Can return NULL:** If no more data left after this line

**[8] `return (line);`**
- **What:** Give the line to the caller
- **Caller's responsibility:** They must free(line) when done

**Flow diagram:**
```
┌─────────────────┐
│ get_next_line() │
└────────┬────────┘
         │
    [Validation]
         │
         ▼
┌────────────────┐
│   read_file()  │ ← Read until we have '\n' or EOF
└────────┬───────┘
         │
         ▼
┌────────────────┐
│   ft_line()    │ ← Extract one line
└────────┬───────┘
         │
         ▼
┌────────────────┐
│   ft_next()    │ ← Keep the rest
└────────┬───────┘
         │
         ▼
    Return line
```

---

### Helper Function: `read_file(int fd, char *res)`

**Purpose:** Read chunks from file until we have a complete line (or EOF)

```c
char *read_file(int fd, char *res)
{
    char    *buffer;      // [1]
    int     byte_read;    // [2]

    buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));  // [3]
    if (!buffer)          // [4]
        return (NULL);
    
    byte_read = 1;        // [5]
    
    while (!ft_strchr(res, '\n') && byte_read != 0)  // [6]
    {
        byte_read = read(fd, buffer, BUFFER_SIZE);   // [7]
        
        if (byte_read == -1)  // [8]
        {
            free(buffer);
            free(res);
            return (NULL);
        }
        
        buffer[byte_read] = '\0';  // [9]
        res = ft_strjoin(res, buffer);  // [10]
    }
    
    free(buffer);  // [11]
    return (res);  // [12]
}
```

**Explanation:**

**[1] `char *buffer;`**
- **What:** Temporary storage for each read() call
- **Size:** BUFFER_SIZE bytes
- **Reused:** Same buffer for all reads in this call

**[2] `int byte_read;`**
- **What:** Stores return value of read()
- **Values:** 
  - `> 0`: Number of bytes read
  - `0`: End of file
  - `-1`: Error

**[3] `buffer = ft_calloc((BUFFER_SIZE + 1), sizeof(char));`**
- **What:** Allocate memory for temporary buffer
- **Size:** BUFFER_SIZE + 1 (extra byte for '\0')
- **Why ft_calloc:** Zeros memory automatically (safe for string operations)
- **Example:** BUFFER_SIZE=5 → allocate 6 bytes, all set to '\0'

**[4] `if (!buffer)`**
- **What:** Check if malloc failed
- **When:** Out of memory (rare but possible)
- **Why return NULL:** Can't continue without memory

**[5] `byte_read = 1;`**
- **What:** Initialize to non-zero value
- **Why:** So while loop runs at least once
- **Trick:** Avoids needing a do-while loop

**[6] `while (!ft_strchr(res, '\n') && byte_read != 0)`**
- **What:** Loop until we have a newline OR reach EOF
- **Condition 1 - `!ft_strchr(res, '\n')`:** No '\n' in res yet (incomplete line)
- **Condition 2 - `byte_read != 0`:** Not at end of file
- **Stop when:** We find '\n' OR reach EOF
- **Example:** 
  - res = "Hello" → Continue (no '\n')
  - res = "Hello\n" → Stop (found '\n')
  - byte_read = 0 → Stop (EOF)

**[7] `byte_read = read(fd, buffer, BUFFER_SIZE);`**
- **What:** Read BUFFER_SIZE bytes from file into buffer
- **Return values:**
  - `42`: Read 42 bytes successfully
  - `0`: Reached end of file
  - `-1`: Error occurred
- **Important:** Does NOT add '\0' automatically!

**[8] `if (byte_read == -1)`**
- **What:** Check for read error
- **Causes:** Invalid fd, I/O error, permission denied, etc.
- **Why free both:** Clean up before returning error
- **Why free res:** Caller won't know to free it if we return NULL

**[9] `buffer[byte_read] = '\0';`**
- **What:** Null-terminate the buffer
- **Why:** Make it a valid C string for ft_strjoin
- **Example:** 
  - Read 5 bytes: "Hello" 
  - buffer[5] = '\0' 
  - Now buffer = "Hello\0" (valid string)

**[10] `res = ft_strjoin(res, buffer);`**
- **What:** Concatenate buffer to res
- **Example:**
  - res = "Hello ", buffer = "World"
  - Result: res = "Hello World"
- **Memory:** 
  - Allocates new memory for result
  - Frees old res inside ft_strjoin
  - Returns pointer to new memory

**[11] `free(buffer);`**
- **What:** Free temporary buffer
- **Why:** We're done reading, don't need it anymore
- **Important:** Only free once, after loop ends

**[12] `return (res);`**
- **What:** Return accumulated data
- **Contains:** At least one complete line (with '\n'), or all remaining data (EOF without '\n')

**Visual example:**

```
File: "Hello World\n"
BUFFER_SIZE = 5

Call 1: get_next_line(fd)
  ├─ read_file(fd, NULL)
  │   ├─ res = NULL
  │   ├─ Loop iteration 1:
  │   │   ├─ read(fd, buffer, 5) → "Hello"
  │   │   ├─ res = ft_strjoin(NULL, "Hello") → "Hello"
  │   │   └─ ft_strchr("Hello", '\n') → NULL (no '\n', continue)
  │   ├─ Loop iteration 2:
  │   │   ├─ read(fd, buffer, 5) → " Worl"
  │   │   ├─ res = ft_strjoin("Hello", " Worl") → "Hello Worl"
  │   │   └─ ft_strchr("Hello Worl", '\n') → NULL (no '\n', continue)
  │   ├─ Loop iteration 3:
  │   │   ├─ read(fd, buffer, 5) → "d\n"
  │   │   ├─ res = ft_strjoin("Hello Worl", "d\n") → "Hello World\n"
  │   │   └─ ft_strchr("Hello World\n", '\n') → found! (exit loop)
  │   └─ return "Hello World\n"
  ├─ ft_line("Hello World\n") → "Hello World\n"
  ├─ ft_next("Hello World\n") → NULL (nothing after '\n')
  └─ return "Hello World\n"

Call 2: get_next_line(fd)
  ├─ buffer = NULL (nothing left)
  ├─ read_file(fd, NULL)
  │   ├─ read(fd, buffer, 5) → 0 (EOF)
  │   └─ return NULL
  └─ return NULL
```

---

### Helper Function: `ft_line(char *buffer)`

**Purpose:** Extract one line (up to and including '\n')

```c
char *ft_line(char *buffer)
{
    char    *line;  // [1]
    int     len;    // [2]
    int     j;      // [3]

    len = 0;
    j = 0;
    
    if (!buffer || !buffer[j])  // [4]
        return (NULL);
    
    while (buffer[len] && buffer[len] != '\n')  // [5]
        len++;
    
    line = ft_calloc((len + 2), sizeof(char));  // [6]
    if (!line)  // [7]
        return (NULL);
    
    while (buffer[j] && buffer[j] != '\n')  // [8]
    {
        line[j] = buffer[j];
        j++;
    }
    
    if (buffer[j] && buffer[j] == '\n')  // [9]
        line[j++] = '\n';
    
    line[j] = '\0';  // [10]
    
    return (line);  // [11]
}
```

**Explanation:**

**[1] `char *line;`**
- **What:** Will store the extracted line
- **Memory:** Allocated inside this function

**[2] `int len;`**
- **What:** Length of the line (without '\n')
- **Used for:** Calculating memory allocation size

**[3] `int j;`**
- **What:** Index for copying characters
- **Purpose:** Track position while copying

**[4] `if (!buffer || !buffer[j])`**
- **What:** Check for invalid input
- **Condition 1 - `!buffer`:** NULL pointer (shouldn't happen, but safe)
- **Condition 2 - `!buffer[j]`:** Empty string (buffer[0] == '\0')
- **Why return NULL:** No line to extract

**[5] `while (buffer[len] && buffer[len] != '\n')`**
- **What:** Count characters until '\n' or end of string
- **Example:**
  - buffer = "Hello\nWorld"
  - len counts: H(1), e(2), l(3), l(4), o(5), stops at '\n'
  - len = 5

**[6] `line = ft_calloc((len + 2), sizeof(char));`**
- **What:** Allocate memory for the line
- **Size calculation:** `len + 2`
  - `len`: Characters before '\n'
  - `+1`: For the '\n' itself
  - `+1`: For '\0' terminator
- **Example:** len=5 → allocate 7 bytes
  - `[0-4]`: "Hello"
  - `[5]`: '\n'
  - `[6]`: '\0'
- **Why ft_calloc:** Zeros memory (safe for string operations)

**[7] `if (!line)`**
- **What:** Check malloc failure
- **When:** Out of memory
- **Return NULL:** Signal error

**[8] `while (buffer[j] && buffer[j] != '\n')`**
- **What:** Copy characters from buffer to line (until '\n')
- **Loop:**
  - `buffer[j]`: Not at end of string
  - `buffer[j] != '\n'`: Not at newline
- **Action:** Copy character, increment j

**[9] `if (buffer[j] && buffer[j] == '\n')`**
- **What:** Copy the '\n' if it exists
- **Condition 1 - `buffer[j]`:** Not at end of string
- **Condition 2 - `buffer[j] == '\n'`:** Current char is newline
- **Post-increment:** `j++` happens after assignment
- **Example:**
  - Before: line = "Hello\0\0", j = 5
  - After: line = "Hello\n\0", j = 6

**[10] `line[j] = '\0';`**
- **What:** Null-terminate the string
- **Why:** Make it a valid C string
- **Example:** line = "Hello\n\0"

**[11] `return (line);`**
- **What:** Return the extracted line
- **Caller must:** free(line) when done

**Visual example:**

```
buffer = "Hello World\nNext Line\n"

Step 1: Calculate len
  len = 0
  buffer[0]='H' != '\n' → len=1
  buffer[1]='e' != '\n' → len=2
  ...
  buffer[10]='d' != '\n' → len=11
  buffer[11]='\n' → STOP
  len = 11

Step 2: Allocate
  line = ft_calloc(11 + 2, 1) = 13 bytes
  line = "\0\0\0\0\0\0\0\0\0\0\0\0\0"

Step 3: Copy characters
  j=0: line[0] = 'H'
  j=1: line[1] = 'e'
  ...
  j=10: line[10] = 'd'
  j=11: buffer[11] = '\n' → STOP

Step 4: Copy newline
  line[11] = '\n', j becomes 12

Step 5: Null terminate
  line[12] = '\0'

Result: line = "Hello World\n"
```

---

### Helper Function: `ft_next(char *buffer)`

**Purpose:** Keep the remainder after extracting a line

```c
char *ft_next(char *buffer)
{
    char    *line;  // [1]
    int     len;    // [2]
    int     j;      // [3]

    len = 0;
    j = 0;
    
    while (buffer[len] && buffer[len] != '\n')  // [4]
        len++;
    
    if (!buffer[len])  // [5]
    {
        free(buffer);
        return (NULL);
    }
    
    line = ft_calloc((ft_strlen(buffer) - len + 1), sizeof(char));  // [6]
    if (!line)  // [7]
    {
        free(buffer);
        return (NULL);
    }
    
    len++;  // [8]
    j = 0;
    
    while (buffer[len])  // [9]
        line[j++] = buffer[len++];
    
    free(buffer);  // [10]
    
    return (line);  // [11]
}
```

**Explanation:**

**[1] `char *line;`**
- **What:** Will store the remainder (everything after '\n')
- **Name confusion:** Called "line" but actually stores the "rest"
- **Could be renamed:** "remainder" or "rest" for clarity

**[2] `int len;`**
- **What:** Position of '\n' in buffer
- **Used for:** Finding where to start copying the rest

**[3] `int j;`**
- **What:** Index for copying into new buffer
- **Starts at 0:** We build a new string from the beginning

**[4] `while (buffer[len] && buffer[len] != '\n')`**
- **What:** Find position of '\n'
- **Same as ft_line:** Count characters until newline
- **Example:** buffer = "Hello\nWorld" → len = 5

**[5] `if (!buffer[len])`**
- **What:** Check if there's a '\n'
- **Condition:** `buffer[len] == '\0'` (reached end without finding '\n')
- **Means:** This was the last line, nothing after it
- **Action:** Free buffer, return NULL (no more data)
- **Example:** buffer = "Last line" (no '\n') → return NULLinto new buffer
- **Starts at 0:** We build a new string 

**[6] `line = ft_calloc((ft_strlen(buffer) - len + 1), sizeof(char));`**
- **What:** Allocate memory for the remainder
- **Size calculation:**
  - `ft_strlen(buffer)`: Total length (e.g., 16)
  - `- len`: Subtract position of '\n' (e.g., 6)
  - `+ 1`: For '\0' terminator
- **Example:**
  - buffer = "Hello\nWorld\n" (length 12)
  - len = 5 (position of first '\n')
  - Size = 12 - 5 + 1 = 8
  - Needed: "World\n" (6 chars) + '\0' (1) = 7... Wait!

**Note:** There's an off-by-one here because we need to skip the '\n' itself!
- Actually: Size should be `ft_strlen(buffer) - len` (not + 1)
- But ft_calloc zeros memory, and we have the +1 from not counting the '\n' we skip
- Works out correctly in practice

**[7] `if (!line)`**
- **What:** Check malloc failure
- **Action:** Free buffer, return NULL

**[8] `len++;`**
- **What:** Move past the '\n'
- **Why:** We don't want to include '\n' in the remainder
- **Example:** 
  - Before: len = 5 (pointing to '\n')
  - After: len = 6 (pointing to character after '\n')

**[9] `while (buffer[len])`**
- **What:** Copy everything after '\n' to new buffer
- **Loop:** Until end of string
- **Action:** Copy character, increment both indices

**[10] `free(buffer);`**
- **What:** Free old buffer
- **Why:** We have a new buffer now, don't need the old one
- **Important:** This prevents memory leaks

**[11] `return (line);`**
- **What:** Return the remainder
- **Can be NULL:** If nothing left after the line
- **Stored in static variable:** Will be used in next call

**Visual example:**

```
buffer = "Line1\nLine2\nLine3"

Step 1: Find '\n' position
  len=0: 'L' != '\n'
  len=1: 'i' != '\n'
  ...
  len=5: '\n' → STOP
  len = 5

Step 2: Check if '\n' exists
  buffer[5] = '\n' (not '\0') → Continue

Step 3: Calculate size
  ft_strlen("Line1\nLine2\nLine3") = 17
  Size = 17 - 5 + 1 = 13

Step 4: Allocate
  line = ft_calloc(13, 1)

Step 5: Skip '\n'
  len++ → len = 6

Step 6: Copy remainder
  j=0, len=6: line[0] = buffer[6] = 'L'
  j=1, len=7: line[1] = buffer[7] = 'i'
  ...
  j=10, len=16: line[10] = buffer[16] = '3'
  buffer[17] = '\0' → STOP

Step 7: Free old buffer
  free(buffer)

Result: line = "Line2\nLine3"
         (saved in static variable for next call)
```

---

### Utility Functions

#### `ft_strlen(const char *s)`

```c
size_t ft_strlen(const char *s)
{
    size_t length;

    length = 0;
    if (s == NULL)  // [1]
        return (0);
    while (s[length] != '\0')  // [2]
        length++;
    return (length);
}
```

**[1] NULL check:**
- **What:** Handle NULL pointer safely
- **Why:** ft_strjoin might pass NULL on first call
- **Standard strlen:** Would crash on NULL
- **Our version:** Returns 0 (treat as empty string)

**[2] Count characters:**
- **What:** Standard string length calculation
- **Stops at:** '\0' (null terminator)

---

#### `ft_strchr(const char *s, int c)`

```c
char *ft_strchr(const char *s, int c)
{
    if (!s)  // [1]
        return (NULL);
    
    while (*s != '\0')  // [2]
    {
        if (*s == (char)c)  // [3]
            return ((char *)s);
        s++;
    }
    
    if (c == '\0')  // [4]
        return ((char *)s);
    
    return (NULL);  // [5]
}
```

**[1] NULL check:**
- **What:** Handle NULL pointer
- **Why:** Prevents crash when buffer is NULL initially

**[2] Loop through string:**
- **What:** Check each character
- **Until:** End of string

**[3] Character found:**
- **What:** Return pointer to first occurrence
- **Cast:** `(char)c` because c is int, need to compare as char

**[4] Special case - searching for '\0':**
- **What:** If looking for null terminator
- **Return:** Pointer to the '\0' at end of string
- **Why:** Standard strchr behavior

**[5] Not found:**
- **What:** Character not in string
- **Return:** NULL

---

#### `ft_calloc(size_t nmemb, size_t size)`

```c
void *ft_calloc(size_t nmemb, size_t size)
{
    void    *ptr;
    size_t  i;

    if (nmemb == 0 || size == 0)  // [1]
        return (malloc(0));
    
    if (nmemb > SIZE_MAX / size)  // [2]
        return (NULL);
    
    ptr = malloc(nmemb * size);  // [3]
    if (ptr == NULL)  // [4]
        return (NULL);
    
    i = 0;
    while (i < nmemb * size)  // [5]
    {
        ((unsigned char *)ptr)[i] = '\0';
        i++;
    }
    
    return (ptr);  // [6]
}
```

**[1] Zero size:**
- **What:** Handle edge case
- **Return:** malloc(0) - implementation defined but safe

**[2] Overflow check:**
- **What:** Prevent integer overflow
- **Example:** nmemb = 1000000000, size = 1000000000 → would overflow
- **Check:** Would the multiplication overflow SIZE_MAX?
- **Why:** Security and correctness

**[3] Allocate memory:**
- **What:** Allocate nmemb * size bytes
- **Example:** nmemb=10, size=1 → 10 bytes

**[4] Allocation failed:**
- **What:** malloc returned NULL
- **When:** Out of memory
- **Return:** NULL to signal error

**[5] Zero the memory:**
- **What:** Set all bytes to '\0'
- **Why:** calloc must zero memory (vs malloc which doesn't)
- **Cast:** `(unsigned char *)` to access individual bytes

**[6] Return:**
- **What:** Return pointer to zeroed memory

---

#### `ft_strjoin(char *s1, char *s2)`

```c
char *ft_strjoin(char *s1, char *s2)
{
    char    *str;
    size_t  i;
    size_t  j;

    str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));  // [1]
    if (!str)  // [2]
    {
        free(s1);
        return (NULL);
    }
    
    i = 0;
    j = 0;
    
    if (s1)  // [3]
        while (s1[i])
            str[j++] = s1[i++];
    
    i = 0;
    while (s2[i])  // [4]
        str[j++] = s2[i++];
    
    str[j] = '\0';  // [5]
    
    free(s1);  // [6]
    
    return (str);  // [7]
}
```

**[1] Allocate memory:**
- **What:** Allocate space for both strings + null terminator
- **Size:** len(s1) + len(s2) + 1
- **ft_strlen handles NULL:** Returns 0 if s1 is NULL

**[2] Allocation failed:**
- **What:** malloc returned NULL
- **Important:** Free s1 before returning
- **Why:** Prevents memory leak

**[3] Copy s1:**
- **What:** Copy first string if it exists
- **Check:** `if (s1)` prevents crash if NULL
- **NULL case:** Skip, j stays at 0

**[4] Copy s2:**
- **What:** Copy second string
- **Assumption:** s2 is never NULL (comes from buffer)

**[5] Null terminate:**
- **What:** Add '\0' at end
- **Why:** Make it a valid C string

**[6] Free s1:**
- **What:** Free the old buffer
- **Why:** We don't need it anymore, have new combined string
- **Pattern:** "Consume" s1, return new string

**[7] Return:**
- **What:** Return new combined string
- **Caller:** Will use this as new buffer/res

---

## 🎁 BONUS PART - MULTIPLE FILE DESCRIPTORS

### Key Difference: Array of Static Buffers

**Mandatory:**
```c
static char *buffer = NULL;  // ONE buffer for ONE fd
```

**Bonus:**
```c
static char *stash[1024];    // 1024 buffers (one per fd)
```

### Why Array?

**Problem to solve:**
```c
// Without array (mandatory):
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);

get_next_line(fd1);  // Reads "Line1 from file1"
get_next_line(fd2);  // Reads "Line1 from file2"
get_next_line(fd1);  // ❌ LOST! Previous fd1 data overwritten
```

**With array (bonus):**
```c
// Each fd has its own slot in array:
stash[3] = data from fd 3
stash[4] = data from fd 4
stash[5] = data from fd 5

get_next_line(3);  // Uses stash[3]
get_next_line(5);  // Uses stash[5]
get_next_line(3);  // Uses stash[3] again - data still there!
```

### Modified get_next_line (Bonus)

```c
char *get_next_line(int fd)
{
    static char *stash[1024];  // [1]
    char        *line;

    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)  // [2]
        return (NULL);
    
    stash[fd] = read_file(fd, stash[fd]);  // [3]
    if (!stash[fd])
        return (NULL);
    
    line = ft_line(stash[fd]);  // [4]
    stash[fd] = ft_next(stash[fd]);  // [5]
    
    return (line);
}
```

**[1] `static char *stash[1024];`**
- **What:** Array of 1024 pointers
- **Each element:** One buffer for one file descriptor
- **Initialized:** All to NULL automatically (static array)
- **Memory diagram:**
  ```
  stash[0] = NULL  → stdin  (usually not used)
  stash[1] = NULL  → stdout (not used for reading)
  stash[2] = NULL  → stderr (not used for reading)
  stash[3] = NULL  → first file you open
  stash[4] = NULL  → second file you open
  ...
  stash[1023] = NULL
  ```

**[2] `if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)`**
- **Additional check:** `fd >= 1024`
- **Why:** Array only has 1024 elements (0 to 1023)
- **Access stash[1024]:** Would be out of bounds (segfault!)

**[3] `stash[fd] = read_file(fd, stash[fd]);`**
- **What:** Use the specific buffer for this fd
- **Example:** fd=5 → use stash[5]
- **Independent:** stash[5] doesn't affect stash[7]

**[4] `line = ft_line(stash[fd]);`**
- **What:** Extract line from this fd's buffer

**[5] `stash[fd] = ft_next(stash[fd]);`**
- **What:** Update this fd's buffer
- **Preserves others:** stash[3] unchanged if we're working with fd=5

### Visual Example: Multiple FDs

```c
int fd1 = open("file1.txt", O_RDONLY);  // fd1 = 3
int fd2 = open("file2.txt", O_RDONLY);  // fd2 = 4

// file1.txt: "AAA\nBBB\nCCC"
// file2.txt: "XXX\nYYY\nZZZ"

// Call 1: get_next_line(fd1)
stash[3] = "AAA\nBBB\nCCC"  // after read_file
line = "AAA\n"              // after ft_line
stash[3] = "BBB\nCCC"       // after ft_next
return "AAA\n"

// Call 2: get_next_line(fd2)
stash[4] = "XXX\nYYY\nZZZ"  // after read_file
line = "XXX\n"              // after ft_line
stash[4] = "YYY\nZZZ"       // after ft_next
return "XXX\n"

// Call 3: get_next_line(fd1)  ← Back to fd1!
stash[3] = "BBB\nCCC"       // STILL THERE!
line = "BBB\n"              // after ft_line
stash[3] = "CCC"            // after ft_next
return "BBB\n"

// Call 4: get_next_line(fd2)  ← Back to fd2!
stash[4] = "YYY\nZZZ"       // STILL THERE!
line = "YYY\n"              // after ft_line
stash[4] = "ZZZ"            // after ft_next
return "YYY\n"
```

**Key insight:** Each file has independent state!

### Other Functions (Bonus)

**ALL utility functions are IDENTICAL to mandatory!**
- `ft_strlen`
- `ft_strchr`
- `ft_calloc`
- `ft_strjoin`
- `read_file`
- `ft_line`
- `ft_next`

**Only difference:** get_next_line uses array instead of single variable.

---

## 🎨 DESIGN DECISIONS

### Why This Architecture?

**Question:** Why split into read_file, ft_line, ft_next?

**Answer:** Separation of concerns (each function has ONE job)

```
read_file  → Input:  Read data from file
ft_line    → Process: Extract one line
ft_next    → Process: Prepare for next iteration
```

**Alternative:** Could do everything in get_next_line
```c
char *get_next_line(int fd) {
    // 100+ lines of spaghetti code
    // Hard to debug
    // Hard to test
    // Hard to understand
}
```

**Our approach:** Small, focused functions
- **Easier to debug:** Bug in line extraction? Check ft_line
- **Easier to test:** Test each function independently
- **Easier to understand:** Each function is 20-30 lines
- **Reusable:** Functions can be used in other projects

---

### Why ft_strjoin Frees s1?

**Question:** Why does ft_strjoin free its first parameter?

**Pattern without freeing:**
```c
char *temp = res;
res = ft_strjoin(res, buffer);
free(temp);  // Must remember to free!
```

**Pattern with freeing:**
```c
res = ft_strjoin(res, buffer);  // Automatically frees old res
```

**Advantages:**
- Less code
- Can't forget to free (prevents memory leaks)
- Cleaner syntax

**Disadvantages:**
- Not standard behavior (standard strjoin doesn't free)
- Must document clearly
- Can't reuse s1 after calling

**Why acceptable here:**
- We never need s1 after joining
- Clear pattern: always building up res/buffer
- Prevents memory leaks

---

### Why Static Variable?

**Question:** Why not pass buffer as parameter?

**With parameter:**
```c
char *buffer = NULL;
char *line1 = get_next_line(fd, &buffer);  // Need to pass address
char *line2 = get_next_line(fd, &buffer);
char *line3 = get_next_line(fd, &buffer);
```

**With static:**
```c
char *line1 = get_next_line(fd);  // Cleaner!
char *line2 = get_next_line(fd);
char *line3 = get_next_line(fd);
```

**Advantages:**
- Simpler API (one parameter instead of two)
- Automatic state management
- Matches standard functions like `strtok()`

**Disadvantages:**
- Not thread-safe (but subject doesn't require it)
- Hidden state (less explicit)
- Can't read multiple files simultaneously (solved in bonus)

---

### Why BUFFER_SIZE as Macro?

**Question:** Why not hardcode buffer size?

**With hardcode:**
```c
char buffer[42];  // Fixed!
// Can't change without editing code
```

**With macro:**
```c
gcc -D BUFFER_SIZE=42 ...   // Different sizes for different uses
gcc -D BUFFER_SIZE=8192 ... // Larger for big files
gcc -D BUFFER_SIZE=1 ...    // Smaller for testing
```

**Advantages:**
- **Flexibility:** Change size at compile time
- **Testing:** Can test with size=1 (edge case)
- **Optimization:** Adjust for different scenarios
  - Small BUFFER_SIZE: Less memory
  - Large BUFFER_SIZE: Fewer system calls (faster)

**Real-world usage:**
- **Testing:** BUFFER_SIZE=1 (catches bugs)
- **Normal files:** BUFFER_SIZE=4096 (OS page size)
- **Large files:** BUFFER_SIZE=8192 or more
- **Network:** BUFFER_SIZE=1024 (packet size)

---

### Why ft_calloc Instead of malloc?

**Question:** Why zero the memory?

**With malloc:**
```c
char *buffer = malloc(10);
// buffer = "a#$%^&*()sf" (random garbage!)
// If we don't write to buffer[5], it has garbage
// ft_strlen might read garbage until it finds '\0' somewhere
```

**With ft_calloc:**
```c
char *buffer = ft_calloc(10, 1);
// buffer = "\0\0\0\0\0\0\0\0\0\0" (all zeros)
// Safe for string operations
// ft_strlen will work even if partially filled
```

**Advantages:**
- **Safety:** No garbage values
- **Predictability:** Known initial state
- **String-safe:** Null-terminated by default
- **Debugging:** Easier to spot bugs

**Cost:**
- Slightly slower (must zero memory)
- Usually negligible for small allocations

---

## ❓ COMMON EVALUATOR QUESTIONS

### Q1: "What happens if BUFFER_SIZE is 1?"

**Answer:**
It works perfectly! Just reads one byte at a time.

**Example:**
```
File: "Hi\n"
BUFFER_SIZE = 1

read() call 1: 'H'  → res = "H"
read() call 2: 'i'  → res = "Hi"
read() call 3: '\n' → res = "Hi\n" (found '\n', stop)
Return "Hi\n"
```

**Why test this?**
- Stress test: Most calls to read()
- Edge case: Smallest possible buffer
- Catches bugs: Off-by-one errors, buffer handling mistakes

---

### Q2: "What happens at end of file?"

**Answer:**
`read()` returns 0, we return remaining data (if any), then NULL.

**Example:**
```c
File: "Line1\nLine2"  (no '\n' at end)

Call 1:
  read_file() → "Line1\nLine2"
  ft_line()   → "Line1\n"
  ft_next()   → "Line2"
  return "Line1\n"

Call 2:
  read_file() → read() returns 0 (EOF)
              → Loop exits
              → return "Line2" (remaining data)
  ft_line()   → "Line2" (no '\n' at end, still valid)
  ft_next()   → NULL (no '\n', nothing left)
  return "Line2"

Call 3:
  buffer = NULL
  read_file() → buffer is NULL, read() returns 0
              → return NULL
  return NULL
```

---

### Q3: "Why do you free inside ft_strjoin?"

**Answer:**
It's a memory management pattern that prevents leaks.

**Without freeing:**
```c
res = ft_strjoin(res, buffer);  // Allocates new memory
// Old res is leaked! Memory leak!
```

**With freeing:**
```c
res = ft_strjoin(res, buffer);  // Frees old res, returns new
// Clean! No leak!
```

**Trade-off:** Can't use s1 after calling, but we never need to.

---

### Q4: "What if fd is negative?"

**Answer:**
We check at the beginning and return NULL.

```c
if (fd < 0 || BUFFER_SIZE <= 0)
    return (NULL);
```

**Why?**
- Invalid file descriptor
- `read(negative_fd, ...)` would return -1 (error)
- Better to catch early and return NULL cleanly

---

### Q5: "What's the difference between mandatory and bonus?"

**Answer:**
Single static buffer vs. array of static buffers.

**Mandatory:**
```c
static char *buffer;  // Can only read ONE file at a time
```

**Bonus:**
```c
static char *stash[1024];  // Can read MULTIPLE files simultaneously
```

**Everything else is identical!**

---

### Q6: "Why size `len + 2` in ft_line?"

**Answer:**
Need space for line content + '\n' + '\0'

**Example:**
```
buffer = "Hello\nWorld"
len = 5 (counted "Hello")

Need to store:
[0-4]: "Hello" (5 bytes)
[5]:   '\n'    (1 byte)
[6]:   '\0'    (1 byte)
Total: 7 bytes = len(5) + 2
```

**Common mistake:** Using `len + 1` (would cut off '\n' or '\0')

---

### Q7: "What if file is empty?"

**Answer:**
Returns NULL immediately.

```c
File: "" (empty)

Call 1:
  read_file() → read() returns 0 immediately (EOF)
              → return NULL
  if (!buffer) → TRUE
  return NULL
```

---

### Q8: "What if line is extremely long?"

**Answer:**
Works fine! We keep reading until we find '\n'.

```c
BUFFER_SIZE = 5
Line length = 1000 characters

Loop 1: read 5 bytes, res = 5 chars
Loop 2: read 5 bytes, res = 10 chars
Loop 3: read 5 bytes, res = 15 chars
...
Loop 200: read 5 bytes, res = 1000 chars (finally found '\n')

Return line of 1000 characters
```

**No limit on line length!** (except available memory)

---

### Q9: "Why not use a global variable?"

**Answer:**
Static is better scoped and more secure.

**Global:**
```c
char *buffer;  // Anywhere in program can access/modify
// Another function might accidentally change it!
```

**Static in function:**
```c
static char *buffer;  // Only this function can access
// Protected from outside interference
```

**Advantages:**
- Encapsulation
- No naming conflicts
- Can't be modified elsewhere

---

### Q10: "How do you handle memory leaks?"

**Answer:**
Careful allocation and freeing pattern.

**Rules:**
1. **Every malloc has a free:** 
   - `read_file` frees buffer at end
   - `ft_strjoin` frees s1
   - `ft_next` frees old buffer
   - Caller frees returned line

2. **On error, free before returning:**
   ```c
   if (byte_read == -1) {
       free(buffer);  // Clean up
       free(res);     // Clean up
       return (NULL);
   }
   ```

3. **Test with valgrind:**
   ```bash
   valgrind --leak-check=full ./program
   # Should show: "All heap blocks were freed"
   ```

---

## 🎯 EVALUATION TIPS

### How to Explain Your Code

**1. Start with the big picture:**
> "get_next_line reads one line at a time from a file descriptor. It uses a static buffer to remember where we left off between calls."

**2. Explain the main flow:**
> "When called, it first reads data until we have a complete line, then extracts that line, saves the remainder for next time, and returns the line to the caller."

**3. Go into details when asked:**
> "The static buffer is crucial because..."
> "I chose to free s1 in ft_strjoin because..."

**4. Mention your design decisions:**
> "I split the logic into three functions because it makes testing and debugging easier. Each function has a single responsibility."

**5. Be ready to trace execution:**
> "If I call get_next_line on a file with 'Hello\n', first read_file reads 'Hello\n', then ft_line extracts 'Hello\n', then ft_next returns NULL because nothing's left."

### What Evaluators Look For

✅ **Understanding of concepts:**
- What is a file descriptor?
- How does read() work?
- Why use static variables?

✅ **Code correctness:**
- Handles all edge cases (empty file, no newline, huge lines)
- No memory leaks (valgrind clean)
- No segfaults
- Norminette compliant

✅ **Design decisions:**
- Why this architecture?
- Why these helper functions?
- Trade-offs considered

✅ **Bonus understanding:**
- How does array of buffers work?
- Why is it needed?
- What's the limit (1024)?

### Testing During Evaluation

**They might test:**
```bash
# Different BUFFER_SIZE values
gcc -D BUFFER_SIZE=1 ...
gcc -D BUFFER_SIZE=9999 ...

# Multiple files (bonus)
gcc get_next_line_bonus.c ...

# Memory leaks
valgrind ./test

# Edge cases
# - Empty file
# - File without newline
# - Very long lines
# - Binary files
```

**Be confident:**
- "Yes, it handles that case because..."
- "Let me trace through what happens when..."
- "I tested this with..."

---

## 📝 SUMMARY CHEAT SHEET

### Key Points to Remember

| Concept | Quick Explanation |
|---------|------------------|
| **fd** | Number representing an open file |
| **read()** | Reads bytes from fd into buffer |
| **static** | Variable that keeps value between calls |
| **BUFFER_SIZE** | How many bytes to read at once |
| **Mandatory** | One static buffer for one fd |
| **Bonus** | Array of buffers for multiple fds |

### Function Responsibilities

```
get_next_line()  → Orchestrate everything
    ↓
read_file()      → Read until we have a line
    ↓
ft_line()        → Extract the line
    ↓
ft_next()        → Save the remainder
```

### Memory Management Rules

1. **ft_strjoin** frees s1
2. **ft_next** frees old buffer
3. **Caller** frees returned line
4. **On error**: Free everything before returning NULL

### Edge Cases Handled

✅ Empty file → Return NULL  
✅ No newline at EOF → Return line without '\n'  
✅ Very long lines → Keep reading until '\n'  
✅ BUFFER_SIZE=1 → Works byte-by-byte  
✅ Negative fd → Return NULL  
✅ Multiple fds (bonus) → Independent buffers  

---

## 🚀 FINAL ADVICE

**For evaluation:**
1. **Understand, don't memorize:** Know WHY, not just WHAT
2. **Trace examples:** Be able to walk through execution
3. **Explain trade-offs:** Every decision has pros/cons
4. **Be honest:** If you don't know, say "I'm not sure, but I think..."
5. **Test before evaluation:** Make sure everything works!

**Good luck with your evaluation! 🍀**

You've built a solid implementation. Be confident and explain it clearly!
