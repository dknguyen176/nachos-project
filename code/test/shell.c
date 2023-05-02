#include "syscall.h"

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
    int i, status, len;
    SpaceId newProc;
    OpenFileId input = _ConsoleInput;
    OpenFileId output = _ConsoleOutput;
    char buffer[BUFFER_SIZE];
    char prompt[2], ch, msg[BUFFER_SIZE];
    int argc;
    char argv[5][BUFFER_SIZE];

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
            len = _strcpy(msg, "Error, executable not found.\n");
            Write(msg, len, output);
        }
        else
            Join(newProc);
    }

    // for (i = 0; i < 5; ++i)
    // {
    //     _strcpy(buffer, "printtest");
    //     newProc = Exec(buffer);
    //     status = Join(newProc);

    //     PrintInt(status);
    // }
}
