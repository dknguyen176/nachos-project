#include "syscall.h"
// #include "copyright.h"

int main(int argc, char *argv[])
{
    OpenFileId fdsrc, fddst;
    char *filenameSrc;
    char *filenameDst;
    char c;
    char buffer[101];
    int size, i;

    if (argc < 3)
    {
        PrintString("Usage: copy <source> <destination>\n");
        Halt();
    }

    filenameSrc = argv[1];
    filenameDst = argv[2];

    fdsrc = Open(filenameSrc, 0);

    if (Create(filenameDst) == -1)
    {
        // xuất thông báo lỗi tạo tập tin
    }
    else
    {
        // xuất thông báo tạo tập tin thành công
    }

    fddst = Open(filenameDst, 0);

    if (fdsrc == -1 || fddst == -1)
    {
        PrintString("copy failed\n");
        return 0;
    }

    Seek(0, fdsrc);
    Seek(0, fddst);

    Read(buffer, 100, fdsrc);
    Write(buffer, 100, fddst);

    Close(fdsrc);
    Close(fddst);

    PrintString("Copy successfully\n");

    Halt();
}