/* socket.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Open, Connect, Send, Receive, Close
 *
 */

#include "syscall.h"

#define PORT 8080

int main()
{
  int fd, sockID, result;
  char receiveMessage[101];

  fd = Open("text.txt", 0);

  result = Close(fd);

  sockID = OpenSocket();

  result = Connect(sockID, "127.0.0.1", PORT);

  result = Send(sockID, "Hello from client", 17);
  result = Receive(sockID, receiveMessage, 100);
  PrintString(receiveMessage);

  result = CloseSocket(sockID);

  Halt();
  /* not reached */
}
