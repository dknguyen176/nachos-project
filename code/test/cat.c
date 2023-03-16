#include "syscall.h"

int main(int argc, char *argv[])
{
    char *filename, buffer[101];
    OpenFileId fd;

    if (argc < 2)
    {
        PrintString("Usage: cat <filename>");
        Halt();
    }

    filename = argv[1];

    fd = Open(filename, 0);

    // char buffer[101];
    Read(buffer, 100, fd);
    PrintString(buffer);

    Halt();
}
