#ifndef EXCEPTION_UTILITY_H
#define EXCEPTION_UTILITY_H

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "debug.h"

#define MAXFILELENGTH 32

// Some helper function
extern char *User2System(int virtAddr, int limit = -1);
extern int System2User(int virtAddr, int len, char *buffer);
extern int readInt(int reg);
extern char *readChars(int reg);
extern void recoverPC();

extern void SyscallReadString();
extern void SyscallPrintString();
extern void SyscallPrintInt();

// File exception handler
extern void SyscallCreateFile();
extern void SyscallOpenFile();
extern void SyscallCloseFile();
extern void SyscallReadFile();
extern void SyscallWriteFile();
extern void SyscallSeekFile();
extern void SyscallRemoveFile();

// Socket exception handler
extern void SyscallOpenSocket();
extern void SyscallConnectSocket();
extern void SyscallCloseSocket();
extern void SyscallSendSocket();
extern void SyscallReceiveSocket();

#endif // EXCEPTION_UTILITY_H