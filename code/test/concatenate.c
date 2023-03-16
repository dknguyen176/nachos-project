#include "syscall.h"
// #include "copyright.h"

int main(int argc, char *argv[])
{
    OpenFileId fd1, fd2;
    char *filenameSrc1;
    char *filenameSrc2;
    int size1, size2;
    char buffer1[101];
    char buffer2[101];

    if (argc < 3)
    {
        PrintString("Usage: concatenate <file1> <file2>");
        Halt();
    }

    filenameSrc1 = argv[1];
    filenameSrc2 = argv[2];

    fd1 = Open(filenameSrc1, 0);
    fd2 = Open(filenameSrc2, 0);

    if (fd1 == -1 || fd2 == -1)
    {
        PrintString("concat failed");
        return 0;
    }

    size1 = Seek(-1, fd1);
    Seek(0, fd1);
    size2 = Seek(-1, fd2);
    Seek(0, fd2);
    Read(buffer1, size1, fd1);
    Read(buffer2, size2, fd2);
    PrintString(buffer1);
    PrintString(buffer2);

    Close(fd1);
    Close(fd2);

    PrintString("Concat successfully");

    Halt();
}