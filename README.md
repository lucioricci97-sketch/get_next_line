*This project has been created as part of the 42 curriculum by luricci.*

# get_next_line

## Description

The **get_next_line** project consists of implementing a function that reads and returns a single line from a file descriptor each time it is called.  
A line is defined as a sequence of characters ending with a newline character (`\n`) or the end of file (EOF).

The goal of this project is to deepen the understanding of **file descriptors**, **static variables**, **buffered reading**, and **memory management** in C, while handling edge cases such as variable buffer sizes, multiple file descriptors (bonus), and partial reads.

## Features

### Mandatory Part
- Reads a file descriptor line by line
- Handles lines ending with `\n` or EOF
- Uses a configurable `BUFFER_SIZE` at compile time
- Proper memory allocation and deallocation

### Bonus Part
- Supports **multiple file descriptors** simultaneously
- Maintains independent reading states per file descriptor

## Algorithm Explanation

### Overview

The algorithm relies on a **static storage buffer** to preserve unread data between function calls. This allows `get_next_line` to return exactly one line per call, even if the line spans multiple reads or if multiple lines are read at once.

### Detailed Steps

1. **Buffered Reading**
   - Allocate a temporary buffer of size `BUFFER_SIZE`.
   - Read from the file descriptor using `read()` until a newline is found or EOF is reached.

2. **Storage Management**
   - Append newly read data to a static string (one per file descriptor for the bonus part).
   - This storage keeps leftover data that was read but not yet returned.

3. **Line Extraction**
   - Search for a newline character in the storage.
   - If found, extract the line up to and including `\n`.
   - Store the remaining data back into the static buffer.

4. **End of File Handling**
   - If EOF is reached and data remains in storage, return it as the last line.
   - If no data remains, return `NULL`.

### Justification

- **Static variables** are required to preserve state between calls.
- **Dynamic memory allocation** allows flexible handling of variable-length lines.
- Supporting multiple file descriptors ensures correctness in real-world use cases.
- The algorithm minimizes unnecessary reads and respects performance constraints.

## Instructions

### Compilation

The function is compiled with the following flags:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=n
```

Replace `n` with the desired buffer size (default tested with `BUFFER_SIZE = 256`).

### Files

- `get_next_line.c`
- `get_next_line_utils.c`
- `get_next_line_bonus.c`
- `get_next_line_bonus_utils.c`
- `get_next_line.h`

## Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int fd;
    char *line;

    fd = open("example.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Technical Constraints

- Allowed functions: `read`, `malloc`, `free`
- No use of `libft`
- Custom helper functions used:
  - `ft_calloc`
  - `ft_strjoin`
  - `ft_strlen`
  - `ft_strchr`
- Handles memory safely and avoids leaks
- Fully compliant with the 42 Norm

## Resources

### Technical References

- `man 2 read`
- `man 3 malloc`
- GNU C Library documentation
- 42 get_next_line subject PDF

### AI Usage

AI tools were used responsibly for:
- Understanding `read()` behavior and buffering logic
- Managing static variables and multiple file descriptors
- Debugging edge cases (EOF handling, newline detection)
- Writing and structuring this README documentation

All implementation decisions and validations were made by the student.

## License

This project is licensed under the **MIT License**.
