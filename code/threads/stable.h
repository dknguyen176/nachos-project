#ifndef STABLE_H
#define STABLE_H

#include "constant.h"
#include "bitmap.h"

class Semaphore;

class Sem
{
private:
    char name[50];  // The semaphore name
    Semaphore *sem; // Create semaphore for management

public:
    // Initial the Sem object, the started value is null
    // Remember to initial the Sem to use
    Sem(char *na, int i);
    // Destruct the Sem object
    ~Sem();

    void wait();     // Conduct the waiting function
    void signal();   // Release the sempahore
    char *GetName(); // Return the semaphore name
};

class STable
{
private:
    Bitmap *bm; // Manage the free slot
    Sem *semTab[MAX_SEMAPHORE];

public:
    // Initial the Sem object, the started value is null
    // Remember to initial the bm object to use
    STable();
    ~STable();

    int Create(char *name, int init); // Check the semaphore name, create a new one if not already
    int Wait(char *name);             // If the semaphore name already exists, call this->P() to execute it. If not, report an error in Wait, Signal function
    int Signal(char *name);
    int Find(char *name);     // Find the semaphore name
    int FindFreeSlot();       // Find an empty slot
    int FindFreeSlotAndSet(); // Find an empty slot and set it to be used
};

#endif
