#include <stdio.h>  // terminal io and perror
#include <string.h> // string handling
#include <stdlib.h>
#include <sys/socket.h> // socket APIs
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // open, close

#include <signal.h> // signal handling
#include <time.h>   // time

#define PORT 8080

void setupServer()
{
  int serverSocket;
  struct sockaddr_in serverAddress;

  serverAddress.sin_family = AF_INET;                     // IPv4
  serverAddress.sin_port = htons(PORT);                   // port number in network byte order (host-to-network short)
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // localhost (host to network long)

  serverSocket = socket(AF_INET, SOCK_STREAM, 0);         // Type IPv4 socket using TCP protocol

}

int main()
{
  return 0;
}
