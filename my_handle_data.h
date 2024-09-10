#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "protocol.h"

int read_data(calcProtocol *protocol_ptr, int clientSocket)
{
    ssize_t receivedBytes = recv(clientSocket, protocol_ptr, sizeof(calcProtocol), 0);
    if (receivedBytes < 0)
    {
        printf("ERROR: Recevied data less than 0, PROBABLY REMOVE THIS");
        return -1;
    }

    if (receivedBytes >= sizeof(calcProtocol))
    {
        return 0;
    }
    else
    {
        printf("ERROR! NOT OK! protocoll\n");
        return -2;
    }
}