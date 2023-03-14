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
  int fd = Open("text.txt", 0);

  int sockID = OpenSocket();

  int result = Connect(sockID, "127.0.0.1", PORT);

  result = CloseSocket(sockID);

  Halt();
  /* not reached */
}
