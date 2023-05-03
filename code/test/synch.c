/* inc.c
 *	Simple program to test whether the semaphore works.
 *  Also test whether the semaphore table works when using shell to call.
 *
 *	Exec("inc") 3 times.
 *
 *  Arguments: <program> <incfile> <semaphore name>
 */

#include "syscall.h"

#define NUM_PROCESSES 3
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

int main(int argc, char **argv)
{
    int i;
    SpaceId execs[NUM_PROCESSES];
    char _argv[3][BUFFER_SIZE];
    char *semName;
    char *incfile;

    incfile = argv[1];
    semName = argv[2];

    if (CreateSemaphore(semName, 1) == -1)
    {
        print("CreateSemaphore failed\n");
        Exit(-1);
    }

    /* Create incfile */
    _strcpy(_argv[0], "createfile"); // program name
    _strcpy(_argv[1], incfile);      // incfile name
    if (Join(ExecV(2, _argv)) == -1)
    {
        print("CreateFile failed\n");
        Exit(-1);
    }

    /* Test inc */
    _strcpy(_argv[0], "inc");   // program name
    _strcpy(_argv[1], incfile); // incfile name
    _strcpy(_argv[2], semName); // semaphore name

    for (i = 0; i < NUM_PROCESSES; ++i)
        execs[i] = ExecV(3, _argv);

    for (i = 0; i < NUM_PROCESSES; ++i)
        Join(execs[i]);
}
