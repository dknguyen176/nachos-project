#include "syscall.h"
#include "copyright.h"

int main()
{
  // Read from stdin
  /*
  char *buffer[101];
  Read(buffer, 100, 0);
  PrintString(buffer);
  */

  // Read from file
  /*
  OpenFileId fd = Open("text.txt", 0);

  char *buffer[101];
  Read(buffer, 100, fd);
  PrintString(buffer);
  */

  Halt();
}
