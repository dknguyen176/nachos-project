#include "syscall.h"

#define input _ConsoleInput
#define output _ConsoleOutput

/*
 * IMPORTANT NOTE
 *  Small uninitialized global data (8 bytes or fewer) is stored in the .sbss segment.
 *  However, the current coff2noff does not handle the .sbss segment.
 *  So any small global data must be initialized.
 */

int argc = 0;
char argv[5][BUFFER_SIZE];

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

void print(char *str)
{
    int len;
    char msg[BUFFER_SIZE];

    len = _strcpy(msg, str);
    Write(msg, len, output);
}

void split(char *str, int *argc, char argv[][BUFFER_SIZE])
{
    int i = 0, j = 0, k = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            argv[j][k] = '\0';
            j++;
            k = 0;
        }
        else
        {
            argv[j][k] = str[i];
            k++;
        }
        i++;
    }
    argv[j][k] = '\0';
    (*argc) = j + 1;
}

int main()
{
    int status;
    SpaceId newProc;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        print("shell> ");
        Read(buffer, 60, input);
        split(buffer, &argc, argv);
        newProc = ExecV(argc, argv);

        if (newProc == -1)
        {
            print("Error, executable not found.\n");
        }
        else
        {
            status = Join(newProc);
        }
    }

    // for (i = 0; i < 5; ++i)
    // {
    //     _strcpy(buffer, "printtest");
    //     newProc = Exec(buffer);
    //     status = Join(newProc);

    //     PrintInt(status);
    // }
}
