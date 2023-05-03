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
    OpenFileId fdsrc, fddst;
    char *filenameSrc, *filenameDst;
    char buffer[BUFFER_SIZE + 1], msg[BUFFER_SIZE + 1];

    if (argc < 3 || argc > 3)
    {
        len = _strcpy(msg, "Usage: copy <filenameSrc> <filenameDst>\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    filenameSrc = argv[1];
    filenameDst = argv[2];

    fdsrc = Open(filenameSrc, O_RDWR);

    if (Create(filenameDst) == -1)
    {
        len = _strcpy(msg, "Create file error\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
    fddst = Open(filenameDst, O_RDWR);

    if (fdsrc == -1 || fddst == -1)
    {
        len = _strcpy(msg, "Open file error\n");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    Seek(0, fdsrc);
    Seek(0, fddst);

    Read(buffer, BUFFER_SIZE, fdsrc);
    Write(buffer, BUFFER_SIZE, fddst);

    Close(fdsrc);
    Close(fddst);

    len = _strcpy(msg, "Copy file successfully\n");
    Write(msg, len, _ConsoleOutput);
}
