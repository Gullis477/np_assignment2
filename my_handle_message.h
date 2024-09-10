#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "protocol.h"

int sendMessage(void *void_ptr, int clientsocket, size_t size)
{
    ssize_t bytesSent = 0;
    bytesSent = send(clientsocket, void_ptr, size, 0);
    if (bytesSent < 0)
    {
        printf("ERROR:sending message failed!\n");
        return -1;
    }
    return 1;
}

void setMessage(calcMessage *message_ptr, int type, int message, int protocol, int major_version, int minor_version)
{
    message_ptr->type = htons(type);
    message_ptr->message = htonl(message);
    message_ptr->protocol = htons(protocol);
    message_ptr->major_version = htons(major_version);
    message_ptr->minor_version = htons(minor_version);
}