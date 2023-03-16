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
  DEBUG(dbgSys, "\n Reading integer number");

  int num = kernel->machine->ReadRegister(reg);

  return num;
}

char *readChars(int reg)
{
  int virtAddr;
  char *str;
  DEBUG(dbgSys, "\n Reading virtual address of string");

  // Lấy tham số tên tập tin từ thanh ghi reg
  virtAddr = kernel->machine->ReadRegister(reg);
  DEBUG(dbgSys, "\n Reading string.");

  // MaxFileLength là = 32
  str = User2System(virtAddr, MAXFILELENGTH);
  if (str == NULL)
  {
    DEBUG(dbgSys, "\n Not enough memory in system");
    kernel->machine->WriteRegister(2, (int)-1); // trả về lỗi cho chương trình người dùng
    return NULL;
  }

  DEBUG(dbgSys, "\n Finish reading string.");
  return str;
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
