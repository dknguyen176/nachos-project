#include "syscall.h"
// #include "copyright.h"

#define maxlen 32

int main(int argc, char *argv[])
{
    int len;
    char *filename;

    filename = argv[1];

    if (argc < 2)
    {
        PrintString("Usage: delete <filename>");
        Halt();
    }

    if (Remove(filename) == -1)
    {
        // xuất thông báo lỗi xóa tập tin
    }
    else
    {
        // xuất thông báo xóa tập tin thành công
        PrintString("Delete successfully");
    }
    Halt();
}
