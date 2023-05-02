#include "pcb.h"
#include "synch.h"
#include "utility.h"
#include "addrspace.h"

PCB::PCB(int id)
{
    pid = id;
    joinsem = new Semaphore("joinsem", 0);
    exitsem = new Semaphore("exitsem", 0);
    mutex = new Semaphore("mutex", 1);
    thread = NULL;

    exitcode = 0;
    numwait = 0;
}

PCB::~PCB()
{
    if (joinsem != NULL)
        delete joinsem;
    if (exitsem != NULL)
        delete exitsem;
    if (mutex != NULL)
        delete mutex;

    if (thread != NULL)
    {
        thread->Finish();
        if (thread->space != NULL) // free up the physical memory of the process (by calling ~AddrSpace())
            delete thread->space;
    }
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
    ASSERTNOTREACHED();
}

void PCB::JoinWait() { joinsem->P(); } // switch to block state and stop, and wait for JoinRelease to continue

void PCB::ExitWait() { exitsem->P(); } // switch to block state and stop, and wait for ExitRelease to continue

void PCB::JoinRelease() { joinsem->V(); } // release the waiting process, which called JoinWait()

void PCB::ExitRelease() { exitsem->V(); } // release the waiting process

void PCB::IncNumWait()
{
    mutex->P();
    numwait++;
    mutex->V();
}

void PCB::DecNumWait()
{
    mutex->P();
    if (numwait > 0)
        numwait--;
    mutex->V();
}

void PCB::SetExitCode(int ec) { exitcode = ec; }

void PCB::SetParentID(int ppid) { parentID = ppid; }

void PCB::SetFileName(char *fn) { strcpy(filename, fn); }

int PCB::GetID() { return pid; }

int PCB::GetParentID() { return parentID; }

int PCB::GetNumWait() { return numwait; }

int PCB::GetExitCode() { return exitcode; }

char *PCB::GetFileName() { return filename; }
