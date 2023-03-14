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

void SysHalt();

int SysAdd(int op1, int op2);

int SysReadConsole(char *buffer, int limit);

int SysWriteConsole(char *buffer, int limit);

#endif /* ! __USERPROG_KSYSCALL_H__ */
