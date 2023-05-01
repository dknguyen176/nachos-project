#include "syscall.h"

int main()
{
    SpaceId newProc;
    OpenFileId input = _ConsoleInput;
    OpenFileId output = _ConsoleOutput;
    char buffer[BUFFER_SIZE];
    char prompt[2], ch;

    prompt[0] = '-';
    prompt[1] = '-';

    // Write(prompt, 2, output);

    buffer[0] = 'p';
    buffer[1] = 'r';
    buffer[2] = 'i';
    buffer[3] = 'n';
    buffer[4] = 't';
    buffer[5] = 't';
    buffer[6] = 'e';
    buffer[7] = 's';
    buffer[8] = 't';
    buffer[9] = '\0';

    newProc = Exec(buffer);
    Join(newProc);

    /*
    while (1)
    {
        Write(prompt, 2, output);

        Read(buffer, 60, input);

        newProc = Exec(buffer);
        // Join(newProc);
    }
    */
    Halt();
}
