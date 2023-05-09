#include "syscall.h"

#define input _ConsoleInput
#define output _ConsoleOutput

int _strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int main()
{
    SpaceId pid1, pid2, pid3;
    char file1[BUFFER_SIZE];
    char file2[BUFFER_SIZE];
    char file3[BUFFER_SIZE];

    _strcpy(file1, "printA");
    _strcpy(file2, "printB");

    pid1 = Exec(file1);
    pid2 = Exec(file2);

    if (Join(pid1) == -1)
    {
        Write("Join failed\n", 12, output);
        Exit(-1);
    }

    if (Join(pid2) == -1)
    {
        Write("Join failed\n", 12, output);
        Exit(-1);
    }
}