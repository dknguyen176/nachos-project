// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

int readInt(int reg)
{
	DEBUG('a', "\n Reading integer number");

	int num = kernel->machine->ReadRegister(reg);

	return num;
}
char *readFilename(int reg)
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
void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
											/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			/* Modify return point */
			recoverPC();

			return;

			ASSERTNOTREACHED();

			break;

		case SC_Create:
		{
			DEBUG('a', "\n SC_Create call ...");

			char *filename = readFilename(4);
			if (!filename)
				return;
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

			return;

			ASSERTNOTREACHED();

			break;
		}

		case SC_Open:
		{
			DEBUG('a', "\n SC_Open call ...");

			char *filename = readFilename(4);
			if (!filename)
				return;

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

			return;

			ASSERTNOTREACHED();

			break;
		}

		case SC_Close:
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

			return;

			ASSERTNOTREACHED();

			break;
		}

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
