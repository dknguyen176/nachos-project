#include "ptable.h"
#include "synch.h"

PTable::PTable(int size)
{
    ASSERT(size <= MAX_PROCESS);

    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem", 1);
    for (int i = 0; i < size; i++)
        pcb[i] = NULL;

    bm->Mark(0);
    pcb[0] = new PCB(0);
    pcb[0]->SetParentID(-1);
    pcb[0]->SetFileName("shell"); // The first process will run the shell
}

PTable::~PTable()
{
    delete bm;
    delete bmsem;
    for (int i = 0; i < psize; i++)
        if (pcb[i] != NULL)
        {
            delete pcb[i];
            pcb[i] = NULL;
        }
}

int PTable::ExecUpdate(char *filename)
{
    bmsem->P(); // Avoid loading 2 processes at the same time.

    if (filename == NULL) // Check the validity of the program "name".
    {
        printf("PTable::ExecUpdate Error -- File name is NULL\n");
        bmsem->V();
        return -1;
    }

    OpenFile *executable = kernel->fileSystem->Open(filename);
    if (executable == NULL) // Check the existence of the program "name" by calling the Open method of FileSystem class.
    {
        printf("PTable::ExecUpdate Error -- File name is invalid\n");
        bmsem->V();
        return -1;
    }
    kernel->fileSystem->CloseFile(executable->FileDescriptor()); // Close the file after checking its existence.

    if (strcmp(filename, kernel->pTab->GetFileName(kernel->currentThread->processID)) == 0) // Compare program name and currentThread name to make sure this program is not called Exec itself.
    {
        printf("PTable::ExecUpdate Error -- Can't execute itself\n");
        bmsem->V();
        return -1;
    }

    int pid = GetFreeSlotAndSet(); // Find the empty position in the PTable table
    if (pid == -1)
    {
        printf("PTable::ExecUpdate Error -- No empty slot\n");
        bmsem->V();
        return -1;
    }

    pcb[pid] = new PCB(pid);                                 // create a new PCB with processID being the index of this slot
    pcb[pid]->SetParentID(kernel->currentThread->processID); // parentID is processID of currentThread
    pcb[pid]->SetFileName(filename);
    pcb[pid]->Exec(filename, pid); // Call the method Exec of PCB class
    bmsem->V();

    return pid;
}

int PTable::ExecVUpdate(char *filename, int argc, char **argv)
{
    bmsem->P(); // Avoid loading 2 processes at the same time.

    if (filename == NULL) // Check the validity of the program "name".
    {
        printf("PTable::ExecUpdate Error -- File name is NULL\n");
        bmsem->V();
        return -1;
    }

    OpenFile *executable = kernel->fileSystem->Open(filename);
    if (executable == NULL) // Check the existence of the program "name" by calling the Open method of FileSystem class.
    {
        printf("PTable::ExecUpdate Error -- File name is invalid\n");
        bmsem->V();
        return -1;
    }
    kernel->fileSystem->CloseFile(executable->FileDescriptor()); // Close the file after checking its existence.

    if (strcmp(filename, kernel->pTab->GetFileName(kernel->currentThread->processID)) == 0) // Compare program name and currentThread name to make sure this program is not called Exec itself.
    {
        printf("PTable::ExecUpdate Error -- Can't execute itself\n");
        bmsem->V();
        return -1;
    }

    int pid = GetFreeSlotAndSet(); // Find the empty position in the PTable table
    if (pid == -1)
    {
        printf("PTable::ExecUpdate Error -- No empty slot\n");
        bmsem->V();
        return -1;
    }

    pcb[pid] = new PCB(pid);                                 // create a new PCB with processID being the index of this slot
    pcb[pid]->SetParentID(kernel->currentThread->processID); // parentID is processID of currentThread
    pcb[pid]->SetFileName(filename);
    pcb[pid]->ExecV(filename, pid, argc, argv); // Call the method Exec of PCB class
    bmsem->V();

    return pid;
}

int PTable::ExitUpdate(int ec)
{
    int pid = kernel->currentThread->processID;

    if (pid == 0) // If the currentThread is the first process, then halt the machine
    {
        kernel->interrupt->Halt();
        return 0;
    }

    if (!IsExist(pid)) // Check the validity of the processID
    {
        printf("PTable::ExitUpdate Error -- Process %d does not exist\n", pid);
        return -1;
    }

    int parentID = pcb[pid]->GetParentID();
    pcb[pid]->SetExitCode(ec);   // Set exit code for the currentThread
    pcb[parentID]->DecNumWait(); // Decrement numwait
    pcb[pid]->JoinRelease();     // Release the parent process, which called JoinWait()

    pcb[pid]->ExitWait(); // Wait until the parent process calls ExitRelease()

    Remove(pid);
    return ec;
}

int PTable::JoinUpdate(int id)
{
    if (!IsExist(id)) // Check the validity of the processID
    {
        printf("PTable::JoinUpdate Error -- Process %d does not exist\n", id);
        return -1;
    }
    if (pcb[id]->GetParentID() != kernel->currentThread->processID) // Check if the processID is the child of the currentThread
    {
        printf("PTable::JoinUpdate Error -- Process %d is not the child of currentThread\n", id);
        return -1;
    }

    int parentID = pcb[id]->GetParentID();
    pcb[parentID]->IncNumWait();     // Increment numwait
    pcb[id]->JoinWait();             // Wait for the child process to execute.
    int ec = pcb[id]->GetExitCode(); // Get the exit code of the child process
    pcb[id]->ExitRelease();          // Release the child process

    return ec;
}

int PTable::GetFreeSlot()
{
    return bm->Find();
}

int PTable::GetFreeSlotAndSet()
{
    return bm->FindAndSet();
}

bool PTable::IsExist(int pid)
{
    if (pid < 0 || pid >= psize)
        return false;
    if (pcb[pid] == NULL)
        return false;
    return true;
}

void PTable::Remove(int pid)
{
    if (IsExist(pid))
    {
        bm->Clear(pid);
        delete pcb[pid];
        pcb[pid] = NULL;
    }
}

char *PTable::GetFileName(int id)
{
    if (!IsExist(id))
        return NULL;
    return pcb[id]->GetFileName();
}
