#include "syscall.h"

#define input _ConsoleInput
#define output _ConsoleOutput

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
    int argc;
    char argv[5][BUFFER_SIZE];
    int status;
    SpaceId newProc;
    char prompt[2];
    char buffer[BUFFER_SIZE];

    prompt[0] = '-';
    prompt[1] = '-';

    while (1)
    {
        Write(prompt, 2, output);
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
