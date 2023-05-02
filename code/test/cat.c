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
    char *filename, buffer[BUFFER_SIZE + 1], msg[BUFFER_SIZE + 1];
    OpenFileId fd;

    if (argc < 2)
    {
        len = _strcpy(msg, "Usage: cat <filename>\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    filename = argv[1];

    fd = Open(filename, O_RDWR);
    Read(buffer, BUFFER_SIZE, fd);
    Write(buffer, BUFFER_SIZE, _ConsoleOutput);
    Close(fd);

    len = _strcpy(msg, "\nRead file successfully\n");
    Write(msg, len, _ConsoleOutput);
}
