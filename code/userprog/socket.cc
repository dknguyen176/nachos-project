#include "exhandler.h"

void SyscallOpenSocket()
{
  DEBUG(dbgSys, "SC_OpenSocket call ...\n");

  OpenFile *file = kernel->fileSystem->SocketOpen();
  if (!file)
  {
    DEBUG(dbgSys, "Error open socket\n");
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  DEBUG(dbgSys, "Open socket succesfully, file descriptor " << file->FileDescriptor() << "\n");

  kernel->machine->WriteRegister(2, (int)file->FileDescriptor()); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}

void SyscallConnectSocket()
{
  DEBUG(dbgSys, "SC_Connect call ...\n");

  int sockID = readInt(4);
  char *ip = readChars(5, BUFFER_SIZE);
  int port = readInt(6);

  OpenFile *socket = kernel->fileSystem->Find(sockID);
  if (!socket)
  {
    DEBUG(dbgSys, "Socket not open\n");
  }
  else
  {
    DEBUG(dbgSys, "Socket open at " << socket->FileDescriptor() << "\n");
  }

  int result = socket->SocketConnect(ip, port);
  if (result == -1)
  {
    DEBUG(dbgSys, "Error connect socket with id " << sockID << " to " << ip << ":" << port << "\n");
    kernel->machine->WriteRegister(2, (int)-1);
    delete ip;

    /* Modify return point */
    recoverPC();

    return;
  }

  DEBUG(dbgSys, "Connect socket succesfully, socket ID " << sockID << " to " << ip << ":" << port << "\n");

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công
  delete ip;

  /* Modify return point */
  recoverPC();
}

void SyscallCloseSocket()
{
  DEBUG(dbgSys, "SC_CloseSocket call ...\n");

  int sockID = readInt(4);

  int result = kernel->fileSystem->CloseFile(sockID);
  if (result == -1)
  {
    DEBUG(dbgSys, "Error close socket with id " << sockID << "\n");
    kernel->machine->WriteRegister(2, (int)-1);

    /* Modify return point */
    recoverPC();

    return;
  }
  DEBUG(dbgSys, "Close socket succesfully, file descriptor " << sockID << "\n");

  kernel->machine->WriteRegister(2, (int)0); // trả về cho chương trình
  // người dùng thành công

  /* Modify return point */
  recoverPC();
}

void SyscallSendSocket()
{
  int sockID = readInt(4);
  char *buffer = readChars(5, BUFFER_SIZE);
  int len = readInt(6);

  // Check if socket is open
  OpenFile *socket = kernel->fileSystem->Find(sockID);
  if (!socket)
  {
    // Nếu socket không mở thì trả về 0
    kernel->machine->WriteRegister(2, (int)0);
    delete buffer;
    recoverPC();
    return;
  }

  int result = socket->SocketSend(buffer, len);
  if (result == -1)
  {
    DEBUG(dbgSys, "Error send socket with id " << sockID << "\n");
    kernel->machine->WriteRegister(2, (int)-1);
    delete buffer;

    /* Modify return point */
    recoverPC();

    return;
  }

  DEBUG(dbgSys, "Send to socket succesfully, socket ID " << sockID << "\n");

  kernel->machine->WriteRegister(2, (int)result); // trả về cho chương trình
  // người dùng thành công
  delete buffer;

  /* Modify return point */
  recoverPC();
}

void SyscallReceiveSocket()
{
  int virtAddr = kernel->machine->ReadRegister(5);

  int sockID = readInt(4);
  char *buffer = readChars(5, BUFFER_SIZE);
  int len = readInt(6);

  // Check if socket is open
  OpenFile *socket = kernel->fileSystem->Find(sockID);
  if (!socket)
  {
    // Nếu socket không mở thì trả về 0
    kernel->machine->WriteRegister(2, (int)0);
    delete buffer;
    recoverPC();
    return;
  }

  int result = socket->SocketReceive(buffer, len);
  if (result == -1)
  {
    DEBUG(dbgSys, "Error receive socket with id " << sockID << "\n");
    kernel->machine->WriteRegister(2, (int)-1);
    delete buffer;

    /* Modify return point */
    recoverPC();

    return;
  }

  DEBUG(dbgSys, "Receive from socket succesfully, socket ID " << sockID << "\n");

  kernel->machine->WriteRegister(2, (int)result); // trả về cho chương trình

  // Copy buffer to user space
  System2User(virtAddr, len, buffer);

  // người dùng thành công
  delete buffer;

  /* Modify return point */
  recoverPC();
}