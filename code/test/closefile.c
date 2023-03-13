/* open.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that close a file.
 *
 */

#include "syscall.h"

int main()
{

  /*Open a file*/
  int fileDescriptor = Open("text.txt", 0);
  if (fileDescriptor == -1)
  {
    // xuất thông báo lỗi tạo tập tin
  }
  else
  {
    // xuất thông báo tạo tập tin thành công
  }

  /*Close a file*/
  if (Close(fileDescriptor) == -1)
  {
    // xuất thông báo lỗi tạo tập tin
  }
  else
  {
    // xuất thông báo tạo tập tin thành công
  }

  Halt();
  /* not reached */
}
