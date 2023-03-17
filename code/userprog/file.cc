#include "exhandler.h"
#include <stdlib.h>

void SyscallReadString()
{
  DEBUG(dbgSys, "\n SC_ReadString call ...");

  kernel->machine->WriteRegister(6, _ConsoleInput);

  SyscallReadFile();
}

void SyscallPrintString()
{
  DEBUG(dbgSys, "\n SC_PrintString call ...");

  char *s = readChars(4);

  // In xâu ra màn hình
  SysWriteConsole(s, strlen(s));

  delete s;

  /* Modify return point */
  recoverPC();
}

void SyscallPrintInt()
{
  DEBUG(dbgSys, "\n SC_PrintInt call ...");

  int i = readInt(4);

  // In số ra màn hình
  printf("Printed int: %d\n", i);

  /* Modify return point */
  recoverPC();
}

void SyscallCreateFile()
{
  DEBUG(dbgSys, "\n SC_Create call ...");

  char *filename = readChars(4);
  if (!filename)
  {
    recoverPC();
    return;
  }

  DEBUG(dbgSys, "\n File name : '" << filename << "'");

  //  Create file with size = 0
  //  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
  //  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
  //  hành Linux, chúng ta không quản ly trực tiếp các block trên
  //  đĩa cứng cấp phát cho file, việc quản ly các block của file
  //  trên ổ đĩa là một đồ án khác
  if (!kernel->fileSystem->Create(filename))
  {
    DEBUG(dbgSys, "\n Error create file '" << filename << "'");
    kernel->machine->WriteRegister(2, -1);
    delete filename;

    /* Modify return point */
    recoverPC();

    return;
  }
  DEBUG(dbgSys, "\n Successful create file '" << filename << "'");
  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công
  delete filename;

  /* Modify return point */
  recoverPC();
}

void SyscallOpenFile()
{
  DEBUG(dbgSys, "\n SC_Open call ...");

  char *filename = readChars(4);
  if (!filename)
  {
    recoverPC();
    return;
  }

  int type = readInt(5);

  OpenFile *file = kernel->fileSystem->Open(filename, type);
  if (!file)
  {
    DEBUG(dbgSys, "\n Error open file '" << filename << "'");
    kernel->machine->WriteRegister(2, (int)-1);
    delete filename;

    /* Modify return point */
    recoverPC();

    return;
  }
  DEBUG(dbgSys, "\n Open file '" << filename << "' succesfully, file descriptor " << file->FileDescriptor());

  kernel->machine->WriteRegister(2, (int)file->FileDescriptor()); // trả về cho chương trình
  // người dùng thành công
  delete filename;

  /* Modify return point */
  recoverPC();
}

void SyscallCloseFile()
{
  DEBUG(dbgSys, "\n SC_Close call ...");

  int file = readInt(4);

  int result = kernel->fileSystem->_Close(file);
  if (result == -1)
  {
    DEBUG(dbgSys, "\n Error close file with id '" << file << "'");
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  DEBUG(dbgSys, "\n Close file succesfully, file descriptor " << file);

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}

void SyscallReadFile()
{
  DEBUG(dbgSys, "\n SC_Read call ...");

  int virtAddr = (kernel->machine->ReadRegister(4));

  char *buffer = readChars(4);
  int charcount = readInt(5);
  int fid = readInt(6);

  if (buffer == NULL)
  {
    DEBUG(dbgSys, "\n Invalid buffer");
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
      DEBUG(dbgSys, "\n Invalid file descriptor");
      kernel->machine->WriteRegister(2, -1);
      delete buffer;
      recoverPC();
      return;
    }

    // Đọc ra charcount kí tự từ file
    charcount = file->Read(buffer, charcount);
  }

  DEBUG(dbgSys, "\n Read " << charcount << " characters");
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
  DEBUG(dbgSys, "\n SC_Write call ...");

  char *buffer = readChars(4);
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
      DEBUG(dbgSys, "\n Invalid file descriptor");
      kernel->machine->WriteRegister(2, -1);
      delete buffer;
      recoverPC();
      return;
    }

    // Viết ra charcount kí tự vào file
    charcount = file->Write(buffer, charcount);
  }

  DEBUG(dbgSys, "\n Write " << charcount << " characters");
  // Trả về cho chương trình người dùng số lượng ký tự đã viết
  kernel->machine->WriteRegister(2, charcount);

  delete buffer;

  /* Modify return point */
  recoverPC();
}

void SyscallSeekFile()
{
  DEBUG(dbgSys, "\n SC_Seek call ...");

  int pos = readInt(4);
  int fid = readInt(5);

  if (fid == _ConsoleInput || fid == _ConsoleOutput)
  {
    DEBUG(dbgSys, "\n Invalid file descriptor");
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  // Lấy openfile từ file descriptor
  OpenFile *file = kernel->fileSystem->Find(fid);
  if (file == NULL || file->isSocket()) // Nếu file descriptor không hợp lệ hoặc là socket thì trả về lỗi
  {
    DEBUG(dbgSys, "\n Invalid file descriptor");
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
    DEBUG(dbgSys, "\n Invalid position");
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  // Đặt con trỏ file tại vị trí pos
  file->Seek(pos);

  DEBUG(dbgSys, "\n Seek file succesfully, file descriptor " << fid);

  // Trả về vị trí của con trỏ sau khi được dịch chuyển
  kernel->machine->WriteRegister(2, pos);

  /* Modify return point */
  recoverPC();
}

void SyscallRemoveFile()
{
  DEBUG(dbgSys, "\n SC_Remove call ...");

  char *filename = readChars(4);

  if (!filename)
  {
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  OpenFile *file = kernel->fileSystem->Find(filename); // Find use filename
  if (file != nullptr)
  {
    DEBUG(dbgSys, "\n File '" << filename << "' is currently open and cannot be removed.");
    kernel->machine->WriteRegister(2, -1);
    delete filename;
    recoverPC();
    return;
  }

  if (!kernel->fileSystem->Remove(filename))
  {
    DEBUG(dbgSys, "\n Error remove file '" << filename << "'");
    kernel->machine->WriteRegister(2, -1);
    delete filename;
    recoverPC();
    return;
  }

  delete filename;

  /* Modify return point */
  recoverPC();
}
