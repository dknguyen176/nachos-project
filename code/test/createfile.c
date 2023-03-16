#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main(int argc, char *argv[])
{
  // Print argv

  char *filename;

  if (argc < 2)
  {
    PrintString("Usage: createfile <filename>\n");
    Halt();
  }

  filename = argv[1];
  // if (strlen(filename) > maxlen)
  // {
  //   PrintString("File name too long");
  //   Halt();
  // }

  /*Create a file*/
  if (Create(filename) == -1)
  {
    PrintString("Create file error\n");
  }
  else
  {
    PrintString("Create file successful\n");
  }
  Halt();
}