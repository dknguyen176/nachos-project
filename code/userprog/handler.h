#ifndef HANDLER_H
#define HANDLER_H

// Some helper function
extern char *User2System(int virtAddr, int limit = -1);
extern int System2User(int virtAddr, int len, char *buffer);
extern int readInt(int reg);
extern char *readChars(int reg, int limit);
extern char **readCharsArray(int reg, int size);
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

// Multiprogramming exception handler
extern void SyscallExec();
extern void SyscallExecV();
extern void SyscallJoin();
extern void SyscallExit();

// Synchronization exception handler
extern void SyscallCreateSemaphore();
extern void SyscallDestroySemaphore();
extern void SyscallWaitSemaphore();
extern void SyscallSignalSemaphore();

#endif // HANDLER_H
