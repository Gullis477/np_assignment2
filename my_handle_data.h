#include <stdio.h>
#include <sys/socket.h>
#include "protocol.h"

int read_data(void *void_ptr, int clientSocket, ssize_t size)
{
    ssize_t receivedBytes = recv(clientSocket, void_ptr, size, 0);
    if (receivedBytes < 0)
    {
        return -1;
    }

    if (receivedBytes == sizeof(calcProtocol))
    {
        return 0;
    }
    else if (receivedBytes == sizeof(calcMessage))
    {
        calcMessage *message_ptr = (calcMessage *)void_ptr;
        uint16_t message_type = ntohs(message_ptr->type);
        uint32_t message = ntohl(message_ptr->message);
        uint16_t major_version = ntohs(message_ptr->major_version);
        uint16_t minor_version = ntohs(message_ptr->minor_version);
        if (message_type == 2 && message == 2 && major_version == 1 && minor_version == 0)
        {
            printf("ERROR! SERVER SENT NOT OK!\n");
            return -2;
        }
        return 0;
    }

    printf("ERROR!NÅGOT JÄVLIGT GALET HAR HÄNT\n");
    return -3;
}