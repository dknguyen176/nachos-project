#include "pcb.h"
#include "synch.h"
#include "utility.h"
#include "addrspace.h"

PCB::PCB(int id)
{
    if (!id)
        parentID = -1;
    else
        parentID = kernel->currentThread->processID;

    pid = id;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("multex", 1);
    thread = NULL;

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
    joinsem->P(); // switch to block state and stop, and wait for JoinRelease to continue
}

void PCB::ExitWait() // switch to block state and stop, and wait for ExitRelease to continue
{
    exitsem->P();
}

void PCB::JoinRelease() // release the waiting process, which called JoinWait()
{
    joinsem->V();
}

void PCB::ExitRelease()
{
    exitsem->V(); // release the waiting process
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
        printf("PCB::Exec Error -- Not enough memory\n");
        mutex->V();
        return -1;
    }
    thread->processID = pid;                     // Set processID of this thread to id.
    parentID = kernel->currentThread->processID; // Set the parentID of this thread to be the processID of the thread calling Exec

    int *zz = new int;              // create a new int pointer to pass to StartProcess function as parameter
    *zz = pid;                      // note that if we pass &pid, the value of pid will be destroyed after this function, so we need to create a new int pointer
    thread->Fork(StartProcess, zz); // Fork a thread to run StartProcess
    mutex->V();
}

void StartProcess(void *id)
{
    // Use load and run to load the executable file into the address space of the current thread
    int pid = *(int *)id;
    char *filename = kernel->pTab->GetFileName(pid);
    AddrSpace *space = new AddrSpace();
    if (!space->Load(filename))
    {
        printf("\nError loading executable\n");
        return;
    }
    space->Execute();
    kernel->currentThread->Finish();
}