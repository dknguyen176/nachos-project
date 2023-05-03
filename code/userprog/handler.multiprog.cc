#include "handler.h"
#include "main.h"
#include "constant.h"

void SyscallExec()
{
    DEBUG(dbgSys, "SC_Exec call ...\n");

    char *progname = readChars(4, MAX_FILENAME_LENGTH);
    if (!progname)
    {
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "File name : '" << progname << "'\n");

    //  Create a new process using pTab ExecUpdate to execute the program
    int pid = kernel->pTab->ExecUpdate(progname);
    if (pid == -1)
    {
        DEBUG(dbgSys, "Can't exec file '" << progname << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete progname;
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Successful exec file '" << progname << "'\n");
    kernel->machine->WriteRegister(2, pid); // trả về cho chương trình người dùng thành công
    delete progname;

    /* Modify return point */
    recoverPC();
}

void SyscallExecV()
{
    DEBUG(dbgSys, "SC_ExecV call ...\n");

    // char *progname = readChars(4, MAX_FILENAME_LENGTH);
    int argc = readInt(4);
    char **argv = readCharsArray(5, argc);
    char *progname = argv[0];

    if (!progname)
    {
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "File name : '" << progname << "'\n");

    //  Create a new process using pTab ExecUpdate to execute the program
    int pid = kernel->pTab->ExecVUpdate(progname, argc, argv);
    if (pid == -1)
    {
        DEBUG(dbgSys, "Can't exec file '" << progname << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete progname;
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "Successful exec file '" << progname << "'\n");
    kernel->machine->WriteRegister(2, pid); // trả về cho chương trình người dùng thành công
    delete progname;

    /* Modify return point */
    recoverPC();
}

void SyscallJoin()
{
    DEBUG(dbgSys, "SC_Join call ...\n");

    int pid = readInt(4);

    int status = kernel->pTab->JoinUpdate(pid);

    DEBUG(dbgSys, "Successful join file '" << pid << "'\n");
    kernel->machine->WriteRegister(2, status); // trả về cho chương trình người dùng thành công

    /* Modify return point */
    recoverPC();
}

void SyscallExit()
{
    DEBUG(dbgSys, "SC_Exit call ...\n");

    int status = readInt(4);

    kernel->pTab->ExitUpdate(status);

    DEBUG(dbgSys, "Successful exit file '" << status << "'\n");
    kernel->machine->WriteRegister(2, status); // trả về cho chương trình người dùng thành công

    /* Modify return point */
    recoverPC();

    kernel->currentThread->Finish(); // Finish the currentThread
}
