#include "syscall.h"
// #include "copyright.h"

#define maxlen 32

int main()
{
    int len;
    char filename[maxlen + 1];
    /*Create a file*/
    if (Remove("amogus.txt") == -1)
    {
        // xuất thông báo lỗi xóa tập tin
    }
    else
    {
        // xuất thông báo xóa tập tin thành công
    }
    Halt();
}
