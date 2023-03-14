#include "syscall.h"
// #include "copyright.h"

int main()
{
    OpenFileId fdsrc, fddst;
    char *filenameSrc = "text1.txt";
    char *filenameDst = "text2.txt";
    char c;
    char buffer[101];
    // int size, i;

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
        PrintString("copy failed");
        return 0;
    }

    // size = Seek(-1, fdsrc);
    Seek(0, fdsrc);
    Seek(0, fddst);

    // for (i = 0; i < size; i++)
    // {
    //     Read(&c, 1, fdsrc);
    //     Write(&c, 1, fddst);
    // }

    Read(buffer, 100, fdsrc);
    Write(buffer, 100, fddst);

    Close(fdsrc);
    Close(fddst);

    PrintString("Copy successfully");

    Halt();
}