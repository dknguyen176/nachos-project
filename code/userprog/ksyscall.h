/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"

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
  buffer[i] = '\0';
  limit = i;
  return limit;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
