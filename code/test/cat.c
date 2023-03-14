#include "syscall.h"
// #include "copyright.h"

int main()
{
    OpenFileId fd = Open("text.txt", 0);

    char buffer[101];
    Read(buffer, 100, fd);
    PrintString(buffer);

    Halt();
}
