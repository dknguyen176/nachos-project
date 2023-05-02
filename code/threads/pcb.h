#ifndef PCB_H
#define PCB_H

#include "thread.h"

class Semaphore;

struct ProcessArg
{
    int pid;
    int argc;
    char **argv;
};

class PCB
{
private:
    Semaphore *joinsem; // semaphore for join process
    Semaphore *exitsem; // semaphore for exit process
    Semaphore *mutex;   // exclusive access semaphore
    int exitcode;
    int numwait; // the number of join process

    int pid;           // The process ID
    int parentID;      // The parent process’s ID
    char filename[50]; // The process name
    Thread *thread;    // the thread for this process

public:
    PCB(int id); // Constructor
    ~PCB();      // Destructor

    // Load the program has the name is “filename” and the process id is pid
    int Exec(char *name, int pid); // Create a thread with the name is filename and the process id is pid
    int ExecV(char *name, int pid, int argc, char **argv);
    void JoinWait();    // The parent process wait for the child process finishes
    void ExitWait();    // The child process finishes
    void JoinRelease(); // The child process notice the parent process
    void ExitRelease(); // The parent process accept to exit the child process
    void IncNumWait();  // Increase the number of the waiting process
    void DecNumWait();  // Decrease the number of the waiting process

    // Set-Get Methods
    void SetExitCode(int);    // Set the exit code for the process
    void SetParentID(int);    // Set the parent process ID
    void SetFileName(char *); // Set the process name
    int GetID();              // Return the PID of the current process
    int GetNumWait();         // Return the number of the waiting process
    int GetExitCode();        // Return the exitcode
    int GetParentID();        // Return the parent process ID
    char *GetFileName();      // Return the process name
};

void StartProcess(void *arg); // Used to start the process

#endif
