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