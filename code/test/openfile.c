/* open.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that open a file.
 *
 */

#include "syscall.h"

int main()
{

  /*Open a file*/
  if (Open("text.txt", 0) == -1)
  {
    // xuất thông báo lỗi tạo tập tin
  }
  else
  {
    // xuất thông báo tạo tập tin thành công
  }

  // printf("Open test result %d\n", result);

  Halt();
  /* not reached */
}
