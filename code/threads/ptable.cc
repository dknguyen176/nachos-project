#include "ptable.h"

PTable::PTable(int size)
{
    psize = size;
    bm = new Bitmap(size);
    bmsem = new Semaphore("bmsem", 1);
    for (int i = 0; i < size; i++)
        pcb[i] = NULL;
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
    bmsem->P();
    int pid = GetFreeSlot();
    if (pid == -1)
    {
        bmsem->V();
        return -1;
    }
    pcb[pid] = new PCB(pid);
    bm->Mark(pid);
    bmsem->V();
    pcb[pid]->Exec(filename, pid);
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