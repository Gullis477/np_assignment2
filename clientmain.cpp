
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
#include "my_assignment.h"
#include "my_socket.h"
#include "my_handle_message.h"
#include "my_handle_data.h"

// Included to get the support library
#include <calcLib.h>

#include "protocol.h"

int get_calcProtocol(calcProtocol *msg_ptr, int clientSocket)
{
  if (recv(clientSocket, msg_ptr, sizeof(calcProtocol), 0) < 0)
  {
    printf("Error: failed to get message\n");
    return -1;
  }
  return 0;
}

int main(int argc, char *argv[])
{

  char delim[] = ":";
  char *Desthost = strtok(argv[1], delim);
  char *Destport = strtok(NULL, delim);
  int port = atoi(Destport);

  int waiting_seconds = 2;
  int clientSocket = createConnectSocket(Desthost, Destport, waiting_seconds);
  if (clientSocket < 0)
  {
    return -1;
  }

  // send first calcMessage, should get back a protocoll

  int attempts = 3;
  int counter = 0;
  bool flag = false;

  calcProtocol protocol;
  calcProtocol *protocol_ptr = &protocol;
  while (counter < attempts)
  {
    calcMessage message;
    calcMessage *message_ptr = &message;
    setMessage(message_ptr, 22, 0, 17, 1, 0);
    if (sendMessage(message_ptr, clientSocket) < 0)
    {
      close(clientSocket);
      return -1;
    }

    // if the server supports the protocoll, it will respond with a calcProtocoll.
    // if the server does not support it , it will respond with calcMessage type=2, message=2, major_version=1,minor_version=0

    int response = read_data(protocol_ptr, clientSocket);
    if (response == 0)
    {
      doAssignment(protocol_ptr);
      counter = 3;
      flag = true;
    }
    else if (response == -2)
    {
      printf("ERROR! server sent NOT OK\n");
      close(clientSocket);
      return 0;
    }
  }
  if (flag == false)
  {
    printf("ERROR! Server timeout 3 times\n");
    close(clientSocket);
    return 0;
  }

  close(clientSocket);
  return 0;
}
