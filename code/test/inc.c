/* inc.c
 *	Simple program to test whether the semaphore works.
 *
 *	Each thread increments the counter in the file "incfile.txt" 5 times.
 *
 *  Arguments: <program> <incfile> <semaphore name>
 */

#include "syscall.h"

#define INCREMENT_TIMES 5

int _strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int _itos(int num, char *buffer)
{
    int i, len;
    char tmp;

    for (i = 0; num > 0; num /= 10)
        buffer[i++] = num % 10 + '0';
    if (i == 0) buffer[i++] = '0';
    buffer[i] = '\0';
    len = i;

    for (i = 0; i < len / 2; i++)
    {
        tmp = buffer[i];
        buffer[i] = buffer[len - i - 1];
        buffer[len - i - 1] = tmp;
    }

    return len;
}

int trim(char *buffer)
{
    int i;

    i = 0;
    while (buffer[i] >= '0' && buffer[i] <= '9') i++;
    if (i == 0) buffer[i++] = '0';
    buffer[i] = '\0';
    return i;
}

int readInt(int fd)
{
    int sz, i, num;
    char buffer[BUFFER_SIZE];

    num = 0;
    sz = Read(buffer, BUFFER_SIZE, fd);
    sz = trim(buffer);

    for (i = 0; i < sz; i++)
        num = num * 10 + buffer[i] - '0';
    return num;
}

void writeInt(int fd, int num)
{
    int len;
    char buffer[BUFFER_SIZE];

    len = _itos(num, buffer);
    Write(buffer, len, fd);
}

int main(int argc, char **argv)
{
    int fd, num, len, i;
    char *filename;
    char *semName;

    filename = argv[1];
    semName = argv[2];

    for (i = 0; i < INCREMENT_TIMES; ++i)
    {
        /* critical section begin */
        Wait(semName);

        fd = Open(filename, O_RDWR);
        num = readInt(fd);

        num++;
        PrintInt(num);

        Seek(0, fd);
        writeInt(fd, num);
        Close(fd);

        Signal(semName);
        /* critical section end */
    }
}
