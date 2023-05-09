#include "syscall.h"

#define input _ConsoleInput
#define output _ConsoleOutput

int main()
{
    int i;

    for (i = 0; i < 3; i++)
    {
        Write("DE", 4, output);
    }
}