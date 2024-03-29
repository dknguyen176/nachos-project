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

int _strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
            return s1[i] < s2[i] ? -1 : 1;
        i++;
    }

    if (s1[i] == '\0' && s2[i] == '\0') return 0;
    if (s1[i] == '\0') return -1;
    return 1;
}

void print(char *str)
{
    int len;
    char msg[BUFFER_SIZE];

    len = _strcpy(msg, str);
    Write(msg, len, output);
}

void split(char *str)
{
    int i = 0, j = 0, k = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            if (k > 0)
            {
                argv[j][k] = '\0';
                j++;
                k = 0;
            }
        }
        else
        {
            argv[j][k] = str[i];
            k++;
        }
        i++;
    }

    if (k > 0)
    {
        argv[j][k] = '\0';
        j++;
    }

    argc = j;
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
        split(buffer);

        if (argc == 0) continue;
        if (argc == 1 && _strcmp(argv[0], "exit") == 0) break;
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
}
