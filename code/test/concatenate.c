#include "syscall.h"
// #include "copyright.h"

int main()
{
    OpenFileId fd1, fd2;
    char *filenameSrc1 = "text1.txt";
    char *filenameSrc2 = "text2.txt";
    char c;
    int size1, size2, i;
    char buffer[101];

    fd1 = Open(filenameSrc1, 0);
    fd2 = Open(filenameSrc2, 0);

    if (fd1 == -1 || fd2 == -1)
    {
        PrintString("concat failed");
        return 0;
    }

    // size = Seek(-1, fdsrc);
    // Seek(0, fdsrc);
    // Seek(0, fddst);

    // for (i = 0; i < size; i++)
    // {
    //     Read(&c, 1, fdsrc);
    //     Write(&c, 1, fddst);
    // }

    size1 = Seek(-1, fd1);
    Seek(0, fd1);
    size2 = Seek(-1, fd2);
    Seek(0, fd2);
    Read(buffer, size1, fd1);
    PrintString(buffer);
    Read(buffer, size2, fd2);
    PrintString(buffer);

    Close(fd1);
    Close(fd2);

    PrintString("Concat successfully");

    Halt();
}