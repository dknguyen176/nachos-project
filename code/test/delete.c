#include "syscall.h"

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

int main(int argc, char *argv[])
{
    int len;
    char *filename, msg[BUFFER_SIZE + 1];

    if (argc < 2)
    {
        len = _strcpy(msg, "Usage: delete <filename>\n");
        Write(msg, len, _ConsoleOutput);
        Halt();
    }

    filename = argv[1];

    if (Remove(filename) == -1)
    {
        len = _strcpy(msg, "Delete file error\n");
        Write(msg, len, _ConsoleOutput);
    }
    else
    {
        len = _strcpy(msg, "Delete file successfully\n");
        Write(msg, len, _ConsoleOutput);
    }
    Halt();
}
