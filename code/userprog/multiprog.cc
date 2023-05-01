#include "exhandler.h"

void SyscallExec()
{
    DEBUG(dbgSys, "SC_Exec call ...\n");

    char *progname = readChars(4, MAXFILELENGTH);
    if (!progname)
    {
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "File name : '" << progname << "'\n");

    //  Check if the file is an executable file
    OpenFile *executable = kernel->fileSystem->Open(progname);
    if (!executable)
    {
        DEBUG(dbgSys, "Can't open file '" << progname << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete progname;
        recoverPC();
        return;
    }

    delete executable;

    //  Create a new process using pTab ExecUpdate to execute the program
    int pid = kernel->pTab->ExecUpdate(progname);

    DEBUG(dbgSys, "Successful exec file '" << progname << "'\n");
    kernel->machine->WriteRegister(2, pid); // trả về cho chương trình
    // người dùng thành công
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
    kernel->machine->WriteRegister(2, status); // trả về cho chương trình
    // người dùng thành công

    /* Modify return point */
    recoverPC();
}

void SyscallExit()
{
    DEBUG(dbgSys, "SC_Exit call ...\n");

    int status = readInt(4);

    kernel->pTab->ExitUpdate(status);

    DEBUG(dbgSys, "Successful exit file '" << status << "'\n");
    kernel->machine->WriteRegister(2, status); // trả về cho chương trình
    // người dùng thành công

    /* Modify return point */
    recoverPC();
}