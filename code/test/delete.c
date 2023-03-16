#include "syscall.h"
// #include "copyright.h"

#define maxlen 32

int main(int argc, char *argv[])
{
    int len, fd;
    char *filename;

    filename = argv[1];

    if (argc < 2)
    {
        PrintString("Usage: delete <filename>\n");
        Halt();
    }

    // fd = Open(filename, 0);

    if (Remove(filename) == -1)
    {
        PrintString("Delete failed\n");
    }
    else
    {
        // xuất thông báo xóa tập tin thành công
        PrintString("Delete successfully\n");
    }
    Halt();
}
