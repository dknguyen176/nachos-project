#include "syscall.h"

int main()
{
    char buffer[BUFFER_SIZE];
    buffer[0] = 'Z';
    buffer[1] = 'e';
    buffer[2] = 'l';
    buffer[3] = 'l';
    buffer[4] = 'o';
    buffer[5] = ' ';
    buffer[6] = 'w';
    buffer[7] = 'o';
    buffer[8] = 'r';
    buffer[9] = 'l';
    buffer[10] = 'd';
    buffer[11] = '\n';
    buffer[12] = '\0';
    Write(buffer, 12, _ConsoleOutput);

    Exit(0);
}
