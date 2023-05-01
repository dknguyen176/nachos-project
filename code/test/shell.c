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

int main()
{
    int len;
    SpaceId newProc;
    OpenFileId input = _ConsoleInput;
    OpenFileId output = _ConsoleOutput;
    char buffer[BUFFER_SIZE];
    char prompt[2], ch, msg[BUFFER_SIZE];

    prompt[0] = '-';
    prompt[1] = '-';

    while (1)
    {
        Write(prompt, 2, output);

        Read(buffer, 60, input);

        newProc = Exec(buffer);
        Join(newProc);

        len = _strcpy(msg, "Program exited\n");
        Write(msg, len, output);
    }
    Exit(0);
}
