#include "syscall.h"

#define PORT 8080

int main(int argc, char *argv[])
{
    int fdsrc, fddst, sockID, result;
    char receiveMessage[101];

    char *srcfilename, filecontent[101], *dstfilename;
    OpenFileId fd;

    if (argc < 3)
    {
        PrintString("Usage: fileclient <filename>\n");
        Halt();
    }

    srcfilename = argv[1];
    dstfilename = argv[2];

    fdsrc = Open(srcfilename, 0);

    if (Create(dstfilename) == -1)
    {
        // xuất thông báo lỗi tạo tập tin
    }
    else
    {
        // xuất thông báo tạo tập tin thành công
    }

    fddst = Open(dstfilename, 0);

    if (fdsrc == -1 || fddst == -1)
    {
        PrintString("Can not open file\n");
        return 0;
    }

    Seek(0, fdsrc);
    Seek(0, fddst);

    Read(filecontent, 100, fdsrc);

    sockID = OpenSocket();

    result = Connect(sockID, "127.0.0.1", PORT);

    // result = Send(sockID, "Hello from client", 17);
    // result = Receive(sockID, receiveMessage, 100);

    // Use Write and Read instead of Send and Receive
    result = Write(filecontent, 17, sockID);
    result = Read(receiveMessage, 100, sockID);

    PrintString(receiveMessage);

    Write(receiveMessage, 100, fddst);

    PrintString("\nFileclient successfuly\n");

    result = CloseSocket(sockID);

    Halt();
    /* not reached */
}
