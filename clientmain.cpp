
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

#include <calcLib.h>

#include "protocol.h"

int main(int argc, char *argv[])
{

  char delim[] = ":";
  char *Desthost = strtok(argv[1], delim);
  char *Destport = strtok(NULL, delim);

  int waiting_seconds = 2;

  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  int status = getaddrinfo(Desthost, Destport, &hints, &res);
  if (status != 0)
  {
    printf("ERROR: Cannot resolve server address\n");
    return -1;
  }

  int clientSocket = createConnectSocket(res, Desthost, Destport, waiting_seconds);
  if (clientSocket < 0)
  {
    freeaddrinfo(res);
    return -1;
  }

  // send first calcMessage, should get back a protocoll

  int attempts = 3;
  int counter = 0;
  bool flag = false;

  calcProtocol protocol;
  calcProtocol *protocol_ptr = &protocol;
  char result_str[20];
  while (counter < attempts)
  {
    calcMessage message;
    calcMessage *message_ptr = &message;
    setMessage(message_ptr, 22, 0, 17, 1, 0);
    if (sendMessage(message_ptr, clientSocket, sizeof(calcMessage), res->ai_addr, res->ai_addrlen) < 0)
    {
      close(clientSocket);
      return -1;
    }

    // if the server supports the protocoll, it will respond with a calcProtocoll.
    // if the server does not support it , it will respond with calcMessage type=2, message=2, major_version=1,minor_version=0

    int response = read_data(protocol_ptr, clientSocket, sizeof(calcProtocol));
    if (response == 0)
    {
      doAssignment(protocol_ptr);
      uint32_t arith = protocol_ptr->arith;

      if (arith < 5)
      {
        int result = ntohl(protocol_ptr->inResult);
        sprintf(result_str, "%d", result);
      }
      else
      {
        sprintf(result_str, "%8.8g", protocol_ptr->flResult);
      }
      counter = 3;
      flag = true;
    }
    else if (response == -2)
    {
      printf("ERROR! server sent NOT OK\n");
      close(clientSocket);
      return 0;
    }
    counter++;
  }
  if (flag == false)
  {
    printf("ERROR! Server timeout 3 times\n");
    close(clientSocket);
    return 0;
  }

  flag = false;
  counter = 0;
  while (counter < attempts)
  {
    if (sendMessage(protocol_ptr, clientSocket, sizeof(calcProtocol), res->ai_addr, res->ai_addrlen) < 0)
    {
      close(clientSocket);
      return -1;
    }

    calcMessage message;
    calcMessage *message_ptr = &message;
    int response = read_data(message_ptr, clientSocket, sizeof(calcMessage));
    if (response == 0)
    {
      counter = 3;
      flag = true;
      int server_message = ntohl(message_ptr->message);
      const char *server_message_char;
      if (server_message == 0)
      {
        server_message_char = "NA";
      }
      else if (server_message == 1)
      {
        server_message_char = "OK";
      }
      else if (server_message == 2)
      {
        server_message_char = "NOT OK";
      }

      printf("%s (myresult=%s)\n", server_message_char, server_message_char);
    }
    if (response == -2)
    {
      close(clientSocket);
      return 0;
    }
    counter++;
  }

  if (flag == false)
  {
    printf("ERROR! Server timeout 3 times\n");
  }
  close(clientSocket);
  return 0;
}
