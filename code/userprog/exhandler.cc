#include "exhandler.h"

char *User2System(int virtAddr, int limit)
{
  int i; // index
  int oneChar;
  if (limit == -1)
  {
    limit = 0;
    do
    {
      kernel->machine->ReadMem(virtAddr + limit, 1, &oneChar);
      limit++;
    } while (oneChar != 0);
  }

  char *kernelBuf = NULL;
  kernelBuf = new char[limit + 1]; // need for terminal string
  if (kernelBuf == NULL)
    return kernelBuf;
  memset(kernelBuf, 0, limit + 1);
  // printf("\n Filename u2s:");
  for (i = 0; i < limit; i++)
  {
    kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
    kernelBuf[i] = (char)oneChar;
    // printf("%c",kernelBuf[i]);
    if (oneChar == 0)
      break;
  }
  return kernelBuf;
}

int System2User(int virtAddr, int len, char *buffer)
{
  if (len < 0)
    return -1;
  if (len == 0)
    return len;
  int i = 0;
  int oneChar = 0;
  do
  {
    oneChar = (int)buffer[i];
    kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
    i++;
  } while (i < len && oneChar != 0);
  return i;
}

int readInt(int reg)
{
  DEBUG('a', "\n Reading integer number");

  int num = kernel->machine->ReadRegister(reg);

  return num;
}

char *readChars(int reg)
{
  int virtAddr;
  char *filename;
  DEBUG('a', "\n Reading virtual address of filename");

  // Lấy tham số tên tập tin từ thanh ghi reg
  virtAddr = kernel->machine->ReadRegister(reg);
  DEBUG('a', "\n Reading filename.");

  // MaxFileLength là = 32
  filename = User2System(virtAddr, MAXFILELENGTH);
  return filename;
  if (filename == NULL)
  {
    printf("\n Not enough memory in system");
    DEBUG('a', "\n Not enough memory in system");
    kernel->machine->WriteRegister(2, -1); // trả về lỗi cho chương trình người dùng
  }
  else
  {
    DEBUG('a', "\n Finish reading filename.");
  }

  return filename;
}

void recoverPC()
{
  /* set previous programm counter (debugging only)*/
  kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

  /* set programm counter to next instruction (all Instructions are 4 byte wide)*/
  kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

  /* set next programm counter for brach execution */
  kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}
