#include "ksyscall.h"

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysReadConsole(char *buffer, int limit)
{
  char c;
  int i = 0;
  do
  {
    c = kernel->synchConsoleIn->GetChar();
    buffer[i] = c;
    i++;
  } while (c != '\n' && i < limit);
  buffer[i - 1] = '\0';
  limit = i;
  return limit;
}

int SysWriteConsole(char *buffer, int limit)
{
  int i;
  for (i = 0; i < limit && buffer[i] != '\0'; i++)
    kernel->synchConsoleOut->PutChar(buffer[i]);
  return i;
}