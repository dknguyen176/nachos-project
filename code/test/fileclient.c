#include "syscall.h"

const char *IP = "127.0.0.1";
const int PORT = 8080;

int _strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int main(int argc, char *argv[])
{
    OpenFileId fd;
    int fdsrc, fddst, sockID, result, len;
    char *srcfilename, *dstfilename;
    char ip[IP_LENGTH + 1];
    char msg[BUFFER_SIZE + 1];
    char filecontent[BUFFER_SIZE + 1];
    char receiveMessage[BUFFER_SIZE + 1];

    if (argc < 3 || argc > 3)
    {
        len = _strcpy(msg, "Usage: fileclient <srcfilename> <dstfilename>");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    srcfilename = argv[1];
    dstfilename = argv[2];

    // Open source file
    fdsrc = Open(srcfilename, O_RDONLY);

    // Open/Create destination file
    if (Create(dstfilename) == -1)
    {
        len = _strcpy(msg, "Can not create file");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
    fddst = Open(dstfilename, O_RDWR);

    if (fdsrc == -1 || fddst == -1)
    {
        len = _strcpy(msg, "Can not open file");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    Seek(0, fdsrc);
    Seek(0, fddst);
    Read(filecontent, BUFFER_SIZE, fdsrc);

    // Connect to server
    _strcpy(ip, IP);
    sockID = OpenSocket();
    if (sockID == -1)
    {
        len = _strcpy(msg, "Can not open socket");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
    if (Connect(sockID, ip, PORT) == -1)
    {
        len = _strcpy(msg, "Can not connect to server");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    // result = Send(sockID, "Hello from client", 17);
    // result = Receive(sockID, receiveMessage, 100);

    // Use Write and Read instead of Send and Receive
    if (Write(filecontent, BUFFER_SIZE, sockID) == -1)
    {
        len = _strcpy(msg, "Can not write to socket");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
    if (Read(receiveMessage, BUFFER_SIZE, sockID) == -1)
    {
        len = _strcpy(msg, "Can not read from socket");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    Write(receiveMessage, BUFFER_SIZE, fddst);

    len = _strcpy(msg, "File transfered\n");
    Write(msg, len, _ConsoleOutput);

    if (Close(fdsrc) == -1 || Close(fddst) == -1)
    {
        len = _strcpy(msg, "Can not close file");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }

    if (CloseSocket(sockID) == -1)
    {
        len = _strcpy(msg, "Can not close socket");
        Write(msg, len, _ConsoleOutput);
        Exit(-1);
    }
}
