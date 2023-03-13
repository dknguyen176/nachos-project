#include "syscall.h"
#include "copyright.h"

int main()
{
    // Write to stdout
    /*
    char *buffer[101];
    Read(buffer, 100, 0);
    Write(buffer, 100, 0);
    PrintString(buffer);
    */

    // Write to file
    /*
    OpenFileId fd = Open("text.txt", 0);
    // OpenFileId fd = Open("text.txt", 1); // Dòng này sẽ báo lỗi vì mở read-only file

    char *buffer[101];
    Read(buffer, 100, 0);
    Write(buffer, 100, fd);
    Close(fd);
    */

    Halt();
}
