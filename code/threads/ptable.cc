#include "ptable.h"
#include "synch.h"

PTable::PTable(int size)
{
    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem", 1);
    for (int i = 0; i < size; i++)
        pcb[i] = NULL;

    bm->Mark(0);
    pcb[0] = new PCB(0);
    pcb[0]->parentID = -1;
    pcb[0]->SetFileName("shell"); // The first process will run the shell
}

PTable::~PTable()
{
    delete bm;
    delete bmsem;
    for (int i = 0; i < psize; i++)
        if (pcb[i] != NULL)
            delete pcb[i];
}

int PTable::ExecUpdate(char *filename)
{
    bmsem->P();           // Avoid loading 2 processes at the same time.
    if (filename == NULL) // Check the validity of the program "name".
    {
        printf("PTable::ExecUpdate Error -- File name is NULL\n");
        bmsem->V();
        return -1;
    }
    if (kernel->fileSystem->Open(filename) == NULL) // Check the existence of the program "name" by calling the Open method of FileSystem class.
    {
        printf("PTable::ExecUpdate Error -- File name is invalid\n");
        bmsem->V();
        return -1;
    }
    if (strcmp(filename, kernel->pTab->GetFileName(kernel->currentThread->processID)) == 0) // Compare program name and currentThread name to make sure this program is not called Exec itself.
    {
        printf("PTable::ExecUpdate Error -- Can't execute itself\n");
        bmsem->V();
        return -1;
    }

    int pid = GetFreeSlot(); // Find the empty position in the Ptable table
    if (pid == -1)
    {
        printf("PTable::ExecUpdate Error -- No empty slot\n");
        bmsem->V();
        return -1;
    }

    pcb[pid] = new PCB(pid); // create a new PCB with processID being the index of this slot
    pcb[pid]->SetFileName(filename);
    pcb[pid]->parentID = kernel->currentThread->processID; // parentID is processID of currentThread
    bm->Mark(pid);                                         // Mark used.
    pcb[pid]->Exec(filename, pid);                         // Call the method Exec of PCB class
    bmsem->V();

    return pid;
}

int PTable::ExitUpdate(int ec)
{
    int pid = kernel->currentThread->processID;
    pcb[pid]->SetExitCode(ec);
    pcb[pid]->ExitRelease();
    pcb[pid]->JoinRelease();
    Remove(pid);
    return 0;
}

int PTable::JoinUpdate(int id)
{
    if (!IsExist(id))
        return -1;
    pcb[id]->JoinWait();
    int ec = pcb[id]->GetExitCode();
    pcb[id]->JoinRelease();
    return ec;
}

int PTable::GetFreeSlot()
{
    return bm->Find();
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
    bmsem->P();
    if (IsExist(pid))
    {
        bm->Clear(pid);
        delete pcb[pid];
        pcb[pid] = NULL;
    }
    bmsem->V();
}

char *PTable::GetFileName(int id)
{
    if (!IsExist(id))
        return NULL;
    return pcb[id]->GetFileName();
}