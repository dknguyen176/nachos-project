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

int main()
{
    char buffer[BUFFER_SIZE];
    int len;

    len = _strcpy(buffer, "Hello world!\n");
    Write(buffer, len, _ConsoleOutput);
}
