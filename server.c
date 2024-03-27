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

  // initialise socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Type IPv4 socket using TCP protocol
  if (serverSocket < 0)
  {
    perror("Error creating socket");
    return -1; 
  }
  
  // enable reuse of port and address
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

  // configure server address to bind socket
  serverAddress.sin_family = AF_INET;                     // IPv4
  serverAddress.sin_port = htons(PORT);                   // port number in network byte order (host-to-network short)
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost (host to network long)

  // next bind socket to server address and listen for connections
  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    printf("Error binding socket to address.\n");
    return 1;
  }

  if (listen(serverSocket, BACKLOG) < 0)
  {
    printf("Error listening on socket.\n");
    return 1;
  }

  return serverSocket;
}

void handleRequest(int clientSocket)
{
  // temp store for the incoming request
  char buffer[1024];

  // attempt to read the request from client
  int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1); // try read sizeof buffer, leave space for null-terminator

  // we want to know how much of bytesRead contains valid data
  // also want to detect the end-of-file
  if (bytesRead < 0)
  {
    perror("Error reading from client socket");
    return;
  }

  // make sure data is null-terminated to make it a valid string
  buffer[bytesRead] = '\0';
  printf("Received request: %s\n", buffer);

  // http response 
  char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\ngreat success 👍👍";

  // send response back to client
  int bytesWritten = write(clientSocket, response, strlen(response));
  if (bytesWritten < 0)
  {
    perror("Error writing to socket");
  }
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
      continue; 
    }

    handleRequest(clientSocket);

    close(clientSocket);
  }

  // close server socket when done
  close(serverSocket);

  return 0;
}
