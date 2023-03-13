/* socket.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Open, Connect, Send, Receive, Close
 *
 */

#include "syscall.h"

int main()
{
  int fd = OpenSocket();

  Halt();
  /* not reached */
}
