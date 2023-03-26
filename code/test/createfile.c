#include "syscall.h"

int _strcpy(char *dest, char *src)
{
  int len = 0;
  while (*src != '\0')
  {
    *dest = *src;
    dest++;
    src++;
    len++;
  }
  *dest = '\0';
  return len;
}

int main(int argc, char *argv[])
{
  int len;
  char *filename;
  char msg[BUFFER_SIZE + 1];

  if (argc < 2)
  {
    len = _strcpy(msg, "Usage: createfile <filename>\n");
    Write(msg, len, _ConsoleOutput);
    Halt();
  }

  filename = argv[1];

  /*Create a file*/
  if (Create(filename) == -1)
  {
    len = _strcpy(msg, "Create file error\n");
    Write(msg, len, _ConsoleOutput);
  }
  else
  {
    len = _strcpy(msg, "Create file successfully\n");
    Write(msg, len, _ConsoleOutput);
  }

  Halt();
}