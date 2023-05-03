#include "handler.h"
#include "main.h"
#include "syscall.h"

void SyscallCreateSemaphore()
{
    DEBUG(dbgSys, "SC_CreateSemaphore call ...\n");

    char *semName = readChars(4, BUFFER_SIZE);
    int semVal = readInt(5);

    if (!semName)
    {
        DEBUG(dbgInfo, "Can't read semaphore name\n");
        recoverPC();
        return;
    }

    DEBUG(dbgInfo, "Semaphore name : '" << semName << "'\n");

    int status = kernel->semTab->Create(semName, semVal);
    if (status == -1)
    {
        DEBUG(dbgInfo, "Can't create semaphore '" << semName << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete semName;
        recoverPC();
        return;
    }

    DEBUG(dbgInfo, "Successful create semaphore '" << semName << "'\n");
    kernel->machine->WriteRegister(2, status);
    delete semName;

    /* Modify return point */
    recoverPC();
}

void SyscallDestroySemaphore() {}

void SyscallWaitSemaphore()
{
    DEBUG(dbgSys, "SC_WaitSemaphore call ...\n");

    char *semName = readChars(4, BUFFER_SIZE);

    if (!semName)
    {
        DEBUG(dbgSys, "Can't read semaphore name\n");
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Semaphore name : '" << semName << "'\n");

    int status = kernel->semTab->Wait(semName);
    if (status == -1)
    {
        DEBUG(dbgSys, "Can't wait semaphore '" << semName << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete semName;
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Successful wait semaphore '" << semName << "'\n");
    kernel->machine->WriteRegister(2, status);
    delete semName;

    /* Modify return point */
    recoverPC();
}

void SyscallSignalSemaphore()
{
    DEBUG(dbgSys, "SC_SignalSemaphore call ...\n");

    char *semName = readChars(4, BUFFER_SIZE);

    if (!semName)
    {
        DEBUG(dbgSys, "Can't read semaphore name\n");
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Semaphore name : '" << semName << "'\n");

    int status = kernel->semTab->Signal(semName);
    if (status == -1)
    {
        DEBUG(dbgSys, "Can't signal semaphore '" << semName << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete semName;
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Successful signal semaphore '" << semName << "'\n");
    kernel->machine->WriteRegister(2, status);
    delete semName;

    /* Modify return point */
    recoverPC();
}
