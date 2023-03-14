#include "exhandler.h"
#include <stdlib.h>

void SyscallPrintString()
{
  DEBUG('a', "\n SC_PrintString call ...");

  char *s = readChars(4);
  if (!s)
  {
    printf("\n Empty string");
    recoverPC();
    return;
  }

  // In xâu ra màn hình
  printf("\n Printed string: %s \n", s);

  delete s;

  /* Modify return point */
  recoverPC();
}

void SyscallPrintInt()
{
  DEBUG('a', "\n SC_PrintInt call ...");

  int i = readInt(4);

  // In số ra màn hình
  printf("\n Printed int: %d \n", i);

  /* Modify return point */
  recoverPC();
}

void SyscallCreateFile()
{
  DEBUG('a', "\n SC_Create call ...");

  char *filename = readChars(4);
  if (!filename)
  {
    recoverPC();
    return;
  }
  // DEBUG('a',"\n File name : '"<<filename<<"'");

  //  Create file with size = 0
  //  Dùng đối tượng fileSystem của lớp OpenFile để tạo file,
  //  việc tạo file này là sử dụng các thủ tục tạo file của hệ điều
  //  hành Linux, chúng ta không quản ly trực tiếp các block trên
  //  đĩa cứng cấp phát cho file, việc quản ly các block của file
  //  trên ổ đĩa là một đồ án khác
  if (!kernel->fileSystem->Create(filename))
  {
    printf("\n Error create file '%s'", filename);
    kernel->machine->WriteRegister(2, -1);
    delete filename;

    /* Modify return point */
    recoverPC();

    return;
  }
  printf("\n Successful create file '%s'", filename);
  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công
  delete filename;

  /* Modify return point */
  recoverPC();
}

void SyscallOpenFile()
{
  DEBUG('a', "\n SC_Open call ...");

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
    printf("\n Error open file '%s'", filename);
    kernel->machine->WriteRegister(2, (int)-1);
    delete filename;

    /* Modify return point */
    recoverPC();

    return;
  }
  printf("\n Open file '%s' succesfully, file descriptor %d\n", filename, file->FileDescriptor());

  kernel->machine->WriteRegister(2, (int)file->FileDescriptor()); // trả về cho chương trình
  // người dùng thành công
  delete filename;

  /* Modify return point */
  recoverPC();
}

void SyscallCloseFile()
{
  DEBUG('a', "\n SC_Close call ...");

  int file = readInt(4);

  int result = kernel->fileSystem->_Close(file);
  if (result == -1)
  {
    printf("\n Error close file with id '%d'", file);
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  printf("\n Close file succesfully, file descriptor %d\n", file);

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}

void SyscallReadFile()
{
  DEBUG('a', "\n SC_Read call ...");

  int virtAddr = (kernel->machine->ReadRegister(4));

  char *buffer = readChars(4);
  int charcount = readInt(5);
  int fid = readInt(6);

  if (buffer == NULL)
  {
    DEBUG('a', "\n Invalid buffer");
    recoverPC();
    return;
  }

  // TODO: Check console output

  // Nếu file descriptor == ConsoleInput thì đọc từ bàn phím
  if (fid == _ConsoleInput)
    charcount = SysReadConsole(buffer, charcount);
  // Nếu file descriptor > 1 và filedescriptor < 20 thì đọc từ file
  else
  {
    // Lấy openfile từ file descriptor
    OpenFile *file = kernel->fileSystem->Find(fid);
    if (file == NULL)
    {
      printf("\n Invalid file descriptor");
      kernel->machine->WriteRegister(2, -1);
      delete buffer;
      recoverPC();
      return;
    }

    // Đọc ra charcount kí tự từ file
    charcount = file->Read(buffer, charcount);
  }

  printf("\n Success reading.");
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
  DEBUG('a', "\n SC_Write call ...");

  char *buffer = readChars(4);
  int charcount = readInt(5);
  int fid = readInt(6);

  // Thu gọn charcount
  charcount = min(charcount, (int)strlen(buffer));

  // TODO: Check console input

  // Nếu file descriptor == ConsoleInput thì viết từ bàn phím
  if (fid == _ConsoleOutput)
    charcount = SysWriteConsole(buffer, charcount);
  // Nếu file descriptor > 1 và filedescriptor < 20 thì viết ra file
  else
  {
    // Lấy openfile từ file descriptor
    OpenFile *file = kernel->fileSystem->Find(fid);
    if (file == NULL)
    {
      printf("\n Invalid file descriptor");
      kernel->machine->WriteRegister(2, -1);
      delete buffer;
      recoverPC();
      return;
    }

    // Viết ra charcount kí tự vào file
    charcount = file->Write(buffer, charcount);
  }

  printf("\n Success writing.");
  // Trả về cho chương trình người dùng số lượng ký tự đã viết
  kernel->machine->WriteRegister(2, charcount);

  delete buffer;

  /* Modify return point */
  recoverPC();
}

void SyscallSeekFile()
{
  DEBUG('a', "\n SC_Seek call ...");

  int pos = readInt(4);
  int fid = readInt(5);

  if (fid == _ConsoleInput || fid == _ConsoleOutput)
  {
    printf("\n Invalid file descriptor");
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  // Lấy openfile từ file descriptor
  OpenFile *file = kernel->fileSystem->Find(fid);
  if (file == NULL)
  {
    printf("\n Invalid file descriptor");
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
    printf("\n Invalid position.");
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  // Đặt con trỏ file tại vị trí pos
  file->Seek(pos);

  printf("\n Success Seek.");

  // Trả về vị trí của con trỏ sau khi được dịch chuyển
  kernel->machine->WriteRegister(2, pos);

  /* Modify return point */
  recoverPC();
}

void SyscallRemoveFile()
{
  char *filename = readChars(4);

  if (!filename)
  {
    kernel->machine->WriteRegister(2, -1);
    recoverPC();
    return;
  }

  // TODO: check if is open
  // OpenFile *file = kernel->fileSystem->Find(filename); // Find use fid
  // if (file != nullptr)
  // {
  //   printf("Error: File '%s' is currently open and cannot be removed.\n", filename);
  //   kernel->machine->WriteRegister(2, -1);
  //   delete filename;
  //   recoverPC();
  //   return;
  // }

  if (!kernel->fileSystem->Remove(filename))
  {
    printf("Error: Could not remove file '%s'\n", filename);
    kernel->machine->WriteRegister(2, -1);
    delete filename;
    recoverPC();
    return;
  }

  delete filename;

  /* Modify return point */
  recoverPC();
}
