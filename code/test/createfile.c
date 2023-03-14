#include "syscall.h"
#include "copyright.h"

#define maxlen 32

int main(int argc, char *argv[])
{

  PrintInt(argc);

  /*Create a file*/
  if (Create("text.txt") == -1)
  {
    PrintString("Create file error");
  }
  else
  {
    PrintString("Create file successful");
  }
  Halt();
}