#include "pcb.h"
#include "synch.h"
#include "utility.h"
#include "addrspace.h"

PCB::PCB(int id)
{
    pid = id;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("multex", 1);

    exitcode = 0;
    numwait = 0;
}

PCB::~PCB()
{
    delete joinsem;
    delete exitsem;
    delete mutex;
}

int PCB::GetID()
{
    return pid;
}

int PCB::GetNumWait()
{
    return numwait;
}

void PCB::JoinWait()
{
    joinsem->P();
}

void PCB::ExitWait()
{
    exitsem->P();
}

void PCB::JoinRelease()
{
    joinsem->V();
}

void PCB::ExitRelease()
{
    exitsem->V();
}

void PCB::IncNumWait()
{
    numwait++;
}

void PCB::DecNumWait()
{
    numwait--;
}

int PCB::GetExitCode()
{
    return exitcode;
}

void PCB::SetExitCode(int ec)
{
    exitcode = ec;
}

void PCB::SetFileName(char *fn)
{
    strcpy(filename, fn);
}

char *PCB::GetFileName()
{
    return filename;
}
int PCB::Exec(char *filename, int pid)
{
    mutex->P(); // Call mutex->P() to help avoid loading 2 processes at the same time
    thread = new Thread(filename);
    if (thread == NULL)
    {
        printf("Not enough memory\n");
        mutex->V();
        return -1;
    }
    thread->processID = pid;                                  // Set processID of this thread is id.
    parentID = kernel->currentThread->processID;              // Set the parentID of this thread to be the processID of the thread calling Exec
    thread->Fork((VoidFunctionPtr)StartProcess, (void *)pid); // Fork a thread to run StartProcess
    mutex->V();
}

void StartProcess(int *pid)
{
    // TODO: Sua cai AddrSpace cua moi process
    /*
    int id = *pid;
    AddrSpace *space = new AddrSpace(id);
    if (space == NULL)
    {
        printf("Not enough memory\n");
        return;
    }
    kernel->currentThread->space = space;
    space->InitRegisters();
    space->RestoreState();
    kernel->machine->Run();
    ASSERTNOTREACHED();
    */
}