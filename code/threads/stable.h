#ifndef STABLE_H
#define STABLE_H

#define MAX_SEMAPHORE 10

#include "bitmap.h"
#include "synch.h"
#include "sem.h"

class STable
{
private:
    Bitmap *bm; // Manage the free slot
    Sem *semTab[MAX_SEMAPHORE];

public:
    // Initial the Sem object, the started value is null
    // Remember to initial the bm object to use
    STable()
    {
        bm = new Bitmap(MAX_SEMAPHORE);
    }
    ~STable()
    {
        if (bm)
            delete bm;
    }
    // Check the semaphore name, create a new one if not already
    // If the semaphore name already exists, call this->P() to execute it. If not, report an error in Wait, Signal function
    int Create(char *name, int init)
    {
        if (Find(name) != -1)
            return -1;
        int id = FindFreeSlot();
        if (id == -1)
            return -1;
        semTab[id] = new Sem(name, init);
        return 0;
    }
    int Wait(char *name)
    {
        int id = Find(name);
        if (id == -1)
            return -1;
        semTab[id]->wait();
        return 0;
    }
    int Signal(char *name)
    {
        int id = Find(name);
        if (id == -1)
            return -1;
        semTab[id]->signal();
        return 0;
    }
    int Find(char *name) // Find the semaphore name
    {
        for (int i = 0; i < MAX_SEMAPHORE; i++)
            if (semTab[i] != NULL && strcmp(semTab[i]->GetName(), name) == 0)
                return i;
        return -1;
    }
    int FindFreeSlot() // Find an empty slot
    {
        return bm->Find();
    }
};

#endif