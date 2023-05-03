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
    char *filename;
    char msg[BUFFER_SIZE + 1];

    if (argc < 2 || argc > 2)
    {
        len = _strcpy(msg, "Usage: createfile <filename>\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    filename = argv[1];

    /*Create a file*/
    if (Create(filename) == -1)
    {
        len = _strcpy(msg, "Create file error\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
    else
    {
        len = _strcpy(msg, "Create file successfully\n");
        Write(msg, len, _ConsoleOutput);
    }
}
