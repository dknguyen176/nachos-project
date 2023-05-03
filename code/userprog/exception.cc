// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#include "handler.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which)
    {
    // Change the code for other exceptions (not system call exceptions) so that the process can
    // complete, rather than halting the machine like before
    // A runtime exception will not cause the operating system to shut down by calling halt()
    case NoException:
        return;
    case PageFaultException:
        DEBUG(dbgSys, "PageFaultException\n");
        return;
    case ReadOnlyException:
        DEBUG(dbgSys, "ReadOnlyException\n");
        return;
    case BusErrorException:
        DEBUG(dbgSys, "BusErrorException\n");
        return;
    case AddressErrorException:
        DEBUG(dbgSys, "AddressErrorException\n");
        return;
    case OverflowException:
        DEBUG(dbgSys, "OverflowException\n");
        return;
    case IllegalInstrException:
        DEBUG(dbgSys, "IllegalInstrException\n");
        return;
    case NumExceptionTypes:
        DEBUG(dbgSys, "NumExceptionTypes\n");
        return;

    case SyscallException:
        switch (type)
        {
        case SC_Halt: {
            DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

            SysHalt();

            ASSERTNOTREACHED();
            break;
        }

        case SC_Add: {
            DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

            /* Process SysAdd Systemcall*/
            int result;
            result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
                            /* int op2 */ (int)kernel->machine->ReadRegister(5));

            DEBUG(dbgSys, "Add returning with " << result << "\n");
            /* Prepare Result */
            kernel->machine->WriteRegister(2, (int)result);

            /* Modify return point */
            recoverPC();

            return;

            ASSERTNOTREACHED();

            break;
        }

        case SC_PrintInt:
            SyscallPrintInt();
            return;

        /* File system operations */
        case SC_Create:
            SyscallCreateFile();
            return;

        case SC_Open:
            SyscallOpenFile();
            return;

        case SC_Close:
            SyscallCloseFile();
            return;

        case SC_Read:
            SyscallReadFile();
            return;

        case SC_Write:
            SyscallWriteFile();
            return;

        case SC_Seek:
            SyscallSeekFile();
            return;

        case SC_Remove:
            SyscallRemoveFile();
            return;

        /* Network operations */
        case SC_OpenSocket:
            SyscallOpenSocket();
            return;

        case SC_Connect:
            SyscallConnectSocket();
            return;

        case SC_CloseSocket:
            SyscallCloseSocket();
            return;

        case SC_Send:
            SyscallSendSocket();
            return;

        case SC_Receive:
            SyscallReceiveSocket();
            return;

        /* Process operations */
        case SC_Exec:
            SyscallExec();
            return;

        case SC_ExecV:
            SyscallExecV();
            return;

        case SC_Join:
            SyscallJoin();
            return;

        case SC_Exit:
            SyscallExit();
            return;

        /* Synchronization operations */
        case SC_CreateSemaphore:
            SyscallCreateSemaphore();
            return;

        case SC_DestroySemaphore:
            SyscallDestroySemaphore();
            return;

        case SC_Wait:
            SyscallWaitSemaphore();
            return;

        case SC_Signal:
            SyscallSignalSemaphore();
            return;

        default:
            cerr << "Unexpected system call " << type << "\n";
            recoverPC();
            return;
        }

        return;

    default:
        cerr << "Unexpected user mode exception" << (int)which << "\n";
        recoverPC();
        return;
    }

    ASSERTNOTREACHED();
}
