#include "exhandler.h"
#include "constant.h"

#include <stdlib.h>

void SyscallPrintInt()
{
    DEBUG(dbgSys, "SC_PrintInt call ...\n");

    int number = readInt(4);

    DEBUG(dbgSys, "Print integer : " << number << "\n");
    // printf("%d", number);

    /* Modify return point */
    recoverPC();
}

void SyscallCreateFile()
{
    DEBUG(dbgSys, "SC_Create call ...\n");

    char *filename = readChars(4, MAX_FILENAME_LENGTH);
    if (!filename)
    {
        recoverPC();
        return;
    }

    DEBUG(dbgSys, "File name : '" << filename << "'\n");

    //  Create file with size = 0
    //  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
    //  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
    //  hành Linux, chúng ta không quản ly trực tiếp các block trên
    //  đĩa cứng cấp phát cho file, việc quản ly các block của file
    //  trên ổ đĩa là một đồ án khác
    if (!kernel->fileSystem->Create(filename))
    {
        DEBUG(dbgSys, "Error create file '" << filename << "'\n");
        kernel->machine->WriteRegister(2, -1);
        delete filename;

        /* Modify return point */
        recoverPC();

        return;
    }
    DEBUG(dbgSys, "Successful create file '" << filename << "'\n");
    kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
    // người dùng thành công
    delete filename;

    /* Modify return point */
    recoverPC();
}

void SyscallOpenFile()
{
    DEBUG(dbgSys, "SC_Open call ...\n");

    char *filename = readChars(4, MAX_FILENAME_LENGTH);
    if (!filename)
    {
        recoverPC();
        return;
    }

    int type = readInt(5);

    OpenFile *file = kernel->fileSystem->Open(filename, type);
    if (!file)
    {
        DEBUG(dbgSys, "Error open file '" << filename << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);
        delete filename;

        /* Modify return point */
        recoverPC();

        return;
    }
    DEBUG(dbgSys, "Open file '" << filename << "' succesfully, file descriptor " << file->FileDescriptor() << "\n");

    kernel->machine->WriteRegister(2, (int)file->FileDescriptor()); // trả về cho chương trình
    // người dùng thành công
    delete filename;

    /* Modify return point */
    recoverPC();
}

void SyscallCloseFile()
{
    DEBUG(dbgSys, "SC_Close call ...\n");

    int file = readInt(4);

    int result = kernel->fileSystem->CloseFile(file);
    if (result == -1)
    {
        DEBUG(dbgSys, "Error close file with id '" << file << "'\n");
        kernel->machine->WriteRegister(2, (int)-1);

        /* Modify return point */
        recoverPC();

        return;
    }
    DEBUG(dbgSys, "Close file succesfully, file descriptor " << file << "\n");

    kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
    // người dùng thành công

    /* Modify return point */
    recoverPC();
}

void SyscallReadFile()
{
    DEBUG(dbgSys, "SC_Read call ...\n");

    int virtAddr = (kernel->machine->ReadRegister(4));

    char *buffer = readChars(4, BUFFER_SIZE);
    int charcount = readInt(5);
    int fid = readInt(6);

    if (buffer == NULL)
    {
        DEBUG(dbgSys, "Invalid buffer\n");
        recoverPC();
        return;
    }

    // Nếu file descriptor == ConsoleInput thì đọc từ bàn phím
    if (fid == _ConsoleInput)
        charcount = SysReadConsole(buffer, charcount);
    else
    {
        // Lấy openfile từ file descriptor
        OpenFile *file = kernel->fileSystem->Find(fid);
        if (file == NULL || fid == _ConsoleOutput) // Nếu file descriptor không hợp lệ hoặc là file descriptor của console output thì báo lỗi
        {
            DEBUG(dbgSys, "Invalid file descriptor\n");
            kernel->machine->WriteRegister(2, -1);
            delete buffer;
            recoverPC();
            return;
        }

        // Đọc ra charcount kí tự từ file
        charcount = file->Read(buffer, charcount);
    }

    DEBUG(dbgSys, "Read " << charcount << " characters\n");
    // Trả về cho chương trình người dùng số lượng ký tự đã đọc
    kernel->machine->WriteRegister(2, charcount);

    // Trả về cho chương trình người dùng buffer
    System2User(virtAddr, charcount, buffer);

    delete buffer;

    /* Modify return point */
    recoverPC();
}

void SyscallWriteFile()
{
    DEBUG(dbgSys, "SC_Write call ...\n");

    char *buffer = readChars(4, BUFFER_SIZE);
    int charcount = readInt(5);
    int fid = readInt(6);

    // Thu gọn charcount
    charcount = min(charcount, (int)strlen(buffer));

    // Nếu file descriptor == ConsoleInput thì viết từ bàn phím
    if (fid == _ConsoleOutput)
        charcount = SysWriteConsole(buffer, charcount);
    else
    {
        // Lấy openfile từ file descriptor
        OpenFile *file = kernel->fileSystem->Find(fid);
        if (file == NULL || fid == _ConsoleInput) // Nếu file descriptor không hợp lệ hoặc là file descriptor của console input thì báo lỗi
        {
            DEBUG(dbgSys, "Invalid file descriptor\n");
            kernel->machine->WriteRegister(2, -1);
            delete buffer;
            recoverPC();
            return;
        }

        // Viết ra charcount kí tự vào file
        charcount = file->Write(buffer, charcount);
    }

    DEBUG(dbgSys, "Write " << charcount << " characters\n");
    // Trả về cho chương trình người dùng số lượng ký tự đã viết
    kernel->machine->WriteRegister(2, charcount);

    delete buffer;

    /* Modify return point */
    recoverPC();
}

void SyscallSeekFile()
{
    DEBUG(dbgSys, "SC_Seek call ...\n");

    int pos = readInt(4);
    int fid = readInt(5);

    if (fid == _ConsoleInput || fid == _ConsoleOutput)
    {
        DEBUG(dbgSys, "Invalid file descriptor\n");
        kernel->machine->WriteRegister(2, -1);
        recoverPC();
        return;
    }

    // Lấy openfile từ file descriptor
    OpenFile *file = kernel->fileSystem->Find(fid);
    if (file == NULL || file->isSocket()) // Nếu file descriptor không hợp lệ hoặc là socket thì trả về lỗi
    {
        DEBUG(dbgSys, "Invalid file descriptor\n");
        kernel->machine->WriteRegister(2, -1);
        recoverPC();
        return;
    }

    // Nếu pos = -1 thì đặt con trỏ file tại vị trí cuối file
    if (pos == -1 || pos > file->Length())
        pos = file->Length();
    // Nếu pos không hợp lệ thì trả về lỗi
    else if (pos < 0)
    {
        DEBUG(dbgSys, "Invalid position\n");
        kernel->machine->WriteRegister(2, -1);
        recoverPC();
        return;
    }

    // Đặt con trỏ file tại vị trí pos
    file->Seek(pos);

    DEBUG(dbgSys, "Seek file succesfully, file descriptor " << fid << "\n");

    // Trả về vị trí của con trỏ sau khi được dịch chuyển
    kernel->machine->WriteRegister(2, pos);

    /* Modify return point */
    recoverPC();
}

void SyscallRemoveFile()
{
    DEBUG(dbgSys, "SC_Remove call ...\n");

    char *filename = readChars(4, MAX_FILENAME_LENGTH);

    if (!filename)
    {
        kernel->machine->WriteRegister(2, -1);
        recoverPC();
        return;
    }

    OpenFile *file = kernel->fileSystem->Find(filename); // Find use filename
    if (file != nullptr)
    {
        DEBUG(dbgSys, "File '" << filename << "' is currently open and cannot be removed\n");
        kernel->machine->WriteRegister(2, -1);
        delete filename;
        recoverPC();
        return;
    }

    if (!kernel->fileSystem->Remove(filename))
    {
        DEBUG(dbgSys, "Error remove file '" << filename << "'\n");
        kernel->machine->WriteRegister(2, -1);
        delete filename;
        recoverPC();
        return;
    }

    delete filename;
    kernel->machine->WriteRegister(2, 0);

    /* Modify return point */
    recoverPC();
}
