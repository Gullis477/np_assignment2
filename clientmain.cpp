
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

// Included to get the support library
#include <calcLib.h>

#include "protocol.h"

int sendMessage(calcMessage *message_ptr, int clientsocket)
{
  ssize_t bytesSent = send(clientsocket, message_ptr, sizeof(calcMessage), 0);
  if (bytesSent < 0)
  {
    printf("ERROR:sending message failed!\n");
    return -1;
  }
  return 1;
}

int createConnectSocket(char *Desthost, char *Destport)
{
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

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
  // connect to server
  if (connect(clientSocket, res->ai_addr, res->ai_addrlen) < 0)
  {
    int port = atoi(Destport);
    printf("ERROR: CANT CONNECT TO %s:%d.\n", Desthost, port);
    freeaddrinfo(res);
    return -1;
  }
  freeaddrinfo(res);
  return clientSocket;
}
int getAdress()
{
}

int get_calcProtocol(calcProtocol *msg_ptr, int clientSocket)
{
  if (recv(clientSocket, msg_ptr, sizeof(calcProtocol), 0) < 0)
  {
    printf("Error: failed to get message\n");
    return -1;
  }
  return 0;
}

void setMessage(calcMessage *message_ptr, int type, int message, int protocol, int major_version, int minor_version)
{
  message_ptr->type = htons(type);
  message_ptr->message = htonl(message);
  message_ptr->protocol = htons(protocol);
  message_ptr->major_version = htons(major_version);
  message_ptr->minor_version = htons(minor_version);
}

struct MyStruct
{
  int field1;
  float field2;
};

int main(int argc, char *argv[])
{
  char *Desthost;
  char *Destport;
  int clientSocket = createConnectSocket(Desthost, Destport);
  if (clientSocket < 0)
  {
    return -1;
  }

  calcMessage message;
  calcMessage *message_ptr = &message;
  setMessage(message_ptr, 22, 0, 17, 1, 0);
  if (sendMessage(message_ptr, clientSocket) < 0)
  {
    return -1;
  }

  calcProtocol protocol;
  calcProtocol *protocol_ptr = &protocol;
  if (get_calcProtocol(protocol_ptr, clientSocket) < 0)
  {
    return -1;
  }

  /* Do magic */
}
