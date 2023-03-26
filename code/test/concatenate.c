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
    OpenFileId fd1, fd2;
    char *filenameSrc1, *filenameSrc2;
    int len, size1, size2;
    char buffer1[BUFFER_SIZE + 1];
    char buffer2[BUFFER_SIZE + 1];
    char msg[BUFFER_SIZE + 1];

    if (argc < 3)
    {
        len = _strcpy(msg, "Usage: concatenate <filename1> <filename2>\n");
        Write(msg, len, _ConsoleOutput);
        Halt();
    }

    filenameSrc1 = argv[1];
    filenameSrc2 = argv[2];

    fd1 = Open(filenameSrc1, 0);
    fd2 = Open(filenameSrc2, 0);

    if (fd1 == -1 || fd2 == -1)
    {
        len = _strcpy(msg, "Open file error\n");
        Write(msg, len, _ConsoleOutput);
        Halt();
    }

    size1 = Seek(-1, fd1);
    size2 = Seek(-1, fd2);
    Seek(0, fd1);
    Seek(0, fd2);

    Read(buffer1, size1, fd1);
    Read(buffer2, size2, fd2);
    Write(buffer1, size1, _ConsoleOutput);
    Write(buffer2, size2, _ConsoleOutput);

    Close(fd1);
    Close(fd2);

    len = _strcpy(msg, "\nConcatenate file successfully\n");
    Write(msg, len, _ConsoleOutput);

    Halt();
}