#include <stdio.h>  // terminal io and perror
#include <string.h> // string handling
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h> // socket APIs
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // open, close

#include <signal.h> // signal handling
#include <time.h>   // time

#define PORT 8080
#define BACKLOG 10 // # of pending connections queue will hold

int setupServer()
{
  int serverSocket;
  struct sockaddr_in serverAddress;

  serverAddress.sin_family = AF_INET;                     // IPv4
  serverAddress.sin_port = htons(PORT);                   // port number in network byte order (host-to-network short)
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost (host to network long)

  serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Type IPv4 socket using TCP protocol

  // reuse port and address
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

  // next bind socket to server address and listen for connections
  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    printf("Error: The server is not bound to the address.\n");
    return 1;
  }

  if (listen(serverSocket, BACKLOG) < 0)
  {
    printf("Error: The server is not listening.\n");
    return 1;
  }

  return 0;
}

int main()
{
  int serverSocket = setupServer();
  if (serverSocket < 0)
  {
    return 1; // exit if server setup failed
  }

  while (1)
  { // accept connections
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // accept a connection (blocking call)
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (clientSocket < 0)
    {
      perror("Error accepting connection");
      continue; // go back to start of loop and wait for another connection
    }

    // handle the request below
    // {

    // }

    close(clientSocket);
  }

  // close server socket when done
  close(serverSocket);

  return 0;
}
