# README

This server listens for HTTP requests on localhost at port 8080 and responds with a basic HTTP response.
I used this program to learn a little about socket programming and the TCP/IP protocol.

## Program Components

- The main function orchestrates the server lifecycle. It starts with the setup and then enters loop that manages incoming connections.
- setupServer implements the initial configuration and preparation of the server to accept connections.
- handleRequest's functionality involves basic interaction with the client, teaching how to read from and write to a socket.

### setupServer

This function initialises the server, configuring it to listen for incoming connections.
- **Creating the Socket:** `socket(AF_INET, SOCK_STREAM, 0)` creates a TCP socket for IPv4 communication.
```C
serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
```
- **Address Reuse:** The socket option is set to `SO_REUSEADDR` to allow the server to bind to a port immediately after it has been used. This avoids "address already in use" errors.
```C
setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
```
- **Configuration:** The server address is configured for IPv4 and to associate the socket with a specific port (`PORT`) and loopback address (`INADDR_LOOPBACK`).

```C
  serverAddress.sin_family = AF_INET;            
  serverAddress.sin_port = htons(PORT);                 
  serverAddress.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 
```
- **Binding:** This is done by associating the socket with a specific port (PORT) and the loopback address using bind().

```C
if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    printf("Error binding socket to address.\n");
    return 1;
  }
```
- This function also allows the socket to listen for incoming connections with a specified backlog queue.

```C
if (listen(serverSocket, BACKLOG) < 0)
  {
    printf("Error listening on socket.\n");
    return 1;
  }
```
- The function concludes by returning `serverSocket`.

### handleRequest

handleRequest handles incoming client requests by reading data from the client socket, generating a basic HTTP response, and sending this response back to the client.
- It does this by:
  - Reading incoming data into a buffer.
  ```C 
  char buffer[1024];
  int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
  ```
  - Checking for errors: 
  ```C
  if (bytesRead < 0)
  {
    perror("Error reading from client socket");
    return;
  }
  ```
  - Ensuring the data is null-terminated:
  ```C
  buffer[bytesRead] = '\0';
  printf("Received request: %s\n", buffer);
  ```
- It then constructs a simple HTTP response message with headers and message body.
```C
  char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\ngreat success 👍👍";
```
- Finally it writes the response back to the client socket after error checking.
```C
int bytesWritten = write(clientSocket, response, strlen(response));
  if (bytesWritten < 0)
  {
    perror("Error writing to socket");
  }
```


### Main Loop

The main function both accepts incoming connections, and delegates request handling to handleRequest.
- **Accepting Connections:** After checking for an error:
  ```C
  int serverSocket = setupServer();
    if (serverSocket < 0)
    {
      return 1; // exit if server setup failed
    }

  ```
  - It enters a loop and initialises a socket and waits for/accepts incoming connections.
  creating a new socket for each connection.
  ```C

  // accept connections
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);

  // accept a connection (blocking call)
  int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
  ```
  - Checks for errors:
  ```C
  if (clientSocket < 0)
  {
    perror("Error accepting connection");
    continue; 
  }
  ```
  - **Request Handling:** The program then passes the client socket to handleRequest for processing, then closes the client socket after request .
  ```C
  handleRequest(clientSocket);

  close(clientSocket);
  ```
