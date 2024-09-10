#include <stdio.h>
#include <sys/socket.h>
#include "protocol.h"

int read_data(void *protocol_ptr, int clientSocket, ssize_t size)
{
    ssize_t receivedBytes = recv(clientSocket, protocol_ptr, size, 0);
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