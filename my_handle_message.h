#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include "protocol.h"

int sendMessage(calcMessage *message_ptr, int clientsocket, int timeout, int attempts)
{
    ssize_t bytesSent = 0;
    bytesSent = send(clientsocket, message_ptr, sizeof(calcMessage), 0);
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