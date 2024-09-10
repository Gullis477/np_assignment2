#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int createConnectSocket(addrinfo *res, char *Desthost, char *Destport, int wait_seconds)
{
    // set up adress
    int status = getaddrinfo(Desthost, Destport, res, &res);
    if (status != 0)
    {
        printf("ERROR: Cannot resolve server address\n");
        return -1;
    }

    int clientSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (clientSocket < 0)
    {
        printf("ERROR: Socket creation failed.\n");
        return -1;
    }

    if (wait_seconds > 0)
    {
        struct timeval timeout;
        timeout.tv_sec = wait_seconds;
        timeout.tv_usec = 0;
        if (setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
        {
            printf("ERROR: Failed to set socket timeout.\n");
            freeaddrinfo(res);
            close(clientSocket);
            return -1;
        }
    }
    freeaddrinfo(res);
    return clientSocket;
}