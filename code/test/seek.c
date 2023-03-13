#include "syscall.h"
#include "copyright.h"

int main()
{
    char buffer[101];

    OpenFileId fd = Open("text.txt", 0);
    Seek(10, fd);
    Read(buffer, 100, fd);
    PrintString(buffer);

    Seek(0, fd);
    Write(buffer, 100, fd);
    Close(fd);

    // Remember to catch return value of Seek for error checking

    Halt();
}
