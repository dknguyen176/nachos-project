#include "stable.h"
#include "synch.h"

Sem::Sem(char *na, int i)
{
    strcpy(this->name, na);
    sem = new Semaphore(this->name, i);
}

Sem::~Sem() // Destruct the Sem object
{
    if (sem)
        delete sem;
}

void Sem::wait() { sem->P(); } // Conduct the waiting function

void Sem::signal() { sem->V(); } // Release semaphore

char *Sem::GetName() { return this->name; } // Return the semaphore name

STable::STable()
{
    bm = new Bitmap(MAX_SEMAPHORE);
}

STable::~STable()
{
    if (bm)
        delete bm;
}

int STable::Create(char *name, int init)
{
    if (Find(name) != -1)
    {
        DEBUG(dbgInfo, "STable::Create Error -- Semaphore name already exists\n")
        return -1;
    }

    int id = FindFreeSlotAndSet();
    if (id == -1)
    {
        DEBUG(dbgInfo, "STable::Create Error -- No free slot\n")
        return -1;
    }

    semTab[id] = new Sem(name, init);
    return 0;
}

int STable::Wait(char *name)
{
    int id = Find(name);
    if (id == -1)
    {
        DEBUG(dbgInfo, "STable::Wait Error -- Semaphore name not found\n")
        return -1;
    }

    semTab[id]->wait();
    return 0;
}

int STable::Signal(char *name)
{
    int id = Find(name);
    if (id == -1)
    {
        DEBUG(dbgInfo, "STable::Signal Error -- Semaphore name not found\n")
        return -1;
    }

    semTab[id]->signal();
    return 0;
}

int STable::Find(char *name) // Find the semaphore name
{
    for (int i = 0; i < MAX_SEMAPHORE; i++)
        if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0)
            return i;
    return -1;
}

int STable::FindFreeSlot() { return bm->Find(); } // Find an empty slot

int STable::FindFreeSlotAndSet() { return bm->FindAndSet(); } // Find an empty slot and set it to be used
