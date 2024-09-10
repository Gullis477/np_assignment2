#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int createConnectSocket(char *Desthost, char *Destport, int wait_seconds)
{

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // set up adress
    int status = getaddrinfo(Desthost, Destport, &hints, &res);
    if (status != 0)
    {
        printf("ERROR: Cannot resolve server address: %s\n", gai_strerror(status));
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
            close(clientSocket); // Close the socket on failure
            return -1;
        }
    }
    freeaddrinfo(res);
    return clientSocket;
}