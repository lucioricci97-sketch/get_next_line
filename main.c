#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;
    int     count;

    count = 0;
    // 1. Open a file (ensure test.txt exists)
    fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file");
        return (1);
    }

    printf("---- START READING ----\n");

    // 2. Call get_next_line in a loop
    while (1)
    {
        line = get_next_line(fd);
        if (line == NULL)
            break;
        count++;
        printf("[%d]: %s", count, line);
        
        // 3. FREE the line after using it (Mandatory!)
        free(line); 
    }

    // 4. Close file and finish
    close(fd);
    printf("---- END OF FILE ----\n");
    return (0);
}