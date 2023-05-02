#include "syscall.h"

const char *IP = "127.0.0.1";
const int PORT = 8080;
const char *msgs[4] = {"Hello from client", "kha nghi", "thIs iS A mEsSaGe", "OpErAtIng sYsTeM"};

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

int main()
{
    int len, result, i;
    int sockID[4];
    char receiveMessage[BUFFER_SIZE + 1];
    char msg[BUFFER_SIZE + 1];
    char ip[IP_LENGTH + 1];

    _strcpy(ip, IP);

    // result = Send(sockID[i], "Hello from client", 17);
    // result = Receive(sockID[i], receiveMessage, 100);

    // Use Write and Read instead of Send and Receive
    for (i = 0; i < 4; i++)
    {
        // Open socket
        sockID[i] = OpenSocket();
        if (sockID[i] == -1)
        {
            len = _strcpy(msg, "Can not open socket");
            Write(msg, len, _ConsoleOutput);
            Exit(-1);
        }

        // Connect to server
        if (Connect(sockID[i], ip, PORT) == -1)
        {
            len = _strcpy(msg, "Can not connect to server");
            Write(msg, len, _ConsoleOutput);
            Exit(-1);
        }

        // Send message
        len = _strcpy(msg, msgs[i]);
        if (Write(msg, len, sockID[i]) == -1)
        {
            len = _strcpy(msg, "Can not write to socket");
            Write(msg, len, _ConsoleOutput);
            Exit(-1);
        }

        // Receive message
        result = Read(receiveMessage, BUFFER_SIZE, sockID[i]);
        if (result == -1)
        {
            len = _strcpy(msg, "Can not read from socket");
            Write(msg, len, _ConsoleOutput);
            Exit(-1);
        }

        // Print result
        Write(receiveMessage, result, _ConsoleOutput);
        Write("\n", 1, _ConsoleOutput);
    }

    // Close socket
    for (i = 0; i < 4; i++)
    {
        if (Close(sockID[i]) == -1)
        {
            len = _strcpy(msg, "Can not close socket");
            Write(msg, len, _ConsoleOutput);
            Exit(-1);
        }
    }

    len = _strcpy(msg, "Client finished\n");
    Write(msg, len, _ConsoleOutput);
}
