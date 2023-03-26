#include "syscall.h"

int _strcpy(char *dest, char *src)
{
    int len = 0;
    while (*src != '\0')
    {
        *dest = *src;
        dest++;
        src++;
        len++;
    }
    *dest = '\0';
    return len;
}

int main(int argc, char *argv[])
{
    int len;
    char *filename, buffer[BUFFER_SIZE + 1], msg[BUFFER_SIZE + 1];
    OpenFileId fd;

    if (argc < 2)
    {
        len = _strcpy(msg, "Usage: cat <filename>\n");
        Write(msg, len, _ConsoleOutput);
        Halt();
    }

    filename = argv[1];

    fd = Open(filename, O_RDWR);
    Read(buffer, BUFFER_SIZE, fd);
    Write(buffer, BUFFER_SIZE, _ConsoleOutput);
    Close(fd);

    len = _strcpy(msg, "\nRead file successfully\n");
    Write(msg, len, _ConsoleOutput);

    Halt();
}
