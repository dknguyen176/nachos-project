#include "exhandler.h"

void SyscallOpenSocket()
{
  DEBUG('a', "\n SC_OpenSocket call ...");

  OpenFile *file = kernel->fileSystem->_OpenSocket();
  if (!file)
  {
    printf("\n Error open socket");
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  printf("\n Open socket succesfully, file descriptor %d\n", file->FileDescriptor());

  kernel->machine->WriteRegister(2, (int)file->FileDescriptor()); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}

void SyscallConnectSocket()
{
  DEBUG('a', "\n SC_Connect call ...");

  int sockID = readInt(4);
  char *ip = readChars(5);
  int port = readInt(6);

  OpenFile *socket = kernel->fileSystem->Find(sockID);
  if (!socket)
    cerr << "Socket is not open\n";
  else
    cerr << "Socket open, fd " << socket->FileDescriptor() << '\n';

  int result = kernel->fileSystem->_Connect(sockID, ip, port);
  if (result == -1)
  {
    printf("\n Error connect socket");
    kernel->machine->WriteRegister(2, (int)-1);
    delete ip;

    /* Modify return point */
    recoverPC();

    return;
  }

  printf("\n Connect socket succesfully, file descriptor %d\n", sockID);

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công
  delete ip;

  /* Modify return point */
  recoverPC();
}

void SyscallCloseSocket()
{
  DEBUG('a', "\n SC_CloseSocket call ...");

  int sockID = readInt(4);

  int result = kernel->fileSystem->_Close(sockID);
  if (result == -1)
  {
    printf("\n Error close socket with id '%d'", sockID);
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  printf("\n Close socket succesfully, file descriptor %d\n", sockID);

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}