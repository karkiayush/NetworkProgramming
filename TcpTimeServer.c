/* It is a simple server application that demonstrates the concept of socket programming.
It listens for a connection from a client and sends the current server time to the client.
*/

#include <stdio.h>      // For the I/O operation like Printf Scanf
#include <stdlib.h>     // For the functions like exit etc.
#include <sys/socket.h> // Define Socket Programming functions like socket, bind, listen, accept etc.
#include <netinet/in.h> // Defines structures like sockaddr_in for IP addresses and ports.
#include <arpa/inet.h>  // Provides functions for converting IP addresses (e.g., inet_ntop, inet_addr).
#include <unistd.h>     // Provide functions like close to manage file descriptors
#include <errno.h>      // contains the perror function for displaying error message
#include <string.h>     // Provide function like memset() & strlen() for memory & string operation
#include <time.h>
#define PORT 9999      // Defines the port number the server will listen on.
#define IP "10.0.2.15" // Defines the IP address of the server.
int main()
{
    printf("Welcome to NP-Socket Lab-1\n");

    /*sockaddr_in is a type in C defined in the <netinet/in.h> header file. It represents an Internet (IPv4) socket address "structure", specifically designed for handling IPv4 network addresses.*/
    int fd, confd;                     // File descriptors for the server and client sockets(client's connection)
    char str[INET_ADDRSTRLEN];         // Buffer to store client IP address in a human readable format
    struct sockaddr_in server, client; // Structure for holding the IP Address & Port information of server & client
    char sendBuff[1024];               // Buffer to hold the message sent to the client
    time_t ticks;                      // Variable to store current time

    // Creating a socket
    /* The socket() function takes 3 things as:
    a. domain b. type c. protocol
------------------------------------------------------------------
    for the domain, we need to specify the address family that the socket will use. Some common values for the domain is:
    i. AF_INET: IPv4 internet protocols
    ii. AF_INET6: IPv6 internet protocols
    iii. AF_UNIX or AF_LOCAL: Local (Unix) communication.
-------------------------------------------------------------------
    for the type, we need to specify the type of socket. Some common values for the type is:
    i. SOCK_STREAM: Provides a reliable, "connection-oriented", "byte-stream communication"(eg: TCP).

    ii. SOCK_DGRAM: Provides a "connectionless", unreliable, datagram communication(eg: UDP).

    iii. SOCK_RAW: provide access ot low-level network protocols(raw sockets)
----------------------------------------------------------------------------
    for the protocol, we need to specify the protocol to be used by socket.client.
    If we want to use the default protocol for the choosen domain and type, we set it to 0. As we're using the IPV4 domain & SOCK_STREAM(i.e. TCP), if we set protocol to 0, then it will act as: IPPROTO_TCP(for TCP).
     */

    // On successful creation of socket, the return value will be non negative integer called the file descriptor, which uniquely identifies the socket.
    // On failure, it returns -1, and sets the global errno variable to indicate the error. We can use the perror function to display a human readable error message.
    fd = socket(
        AF_INET,
        SOCK_STREAM,
        0);
    if (fd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created with descriptor [%d]\n", fd);

    // Initialize server structure and buffers
    memset(&server, 0, sizeof(server));    // clear the server structure to avoid garbage value
    memset(sendBuff, 0, sizeof(sendBuff)); // clear the buffer named as sendBuffer(to avoid garbage value) that is used to hold the message to be sent to client.

    /**********************************Configure server settings*******************************/
    // AF_INET(IPv4) specify the protocol family
    server.sin_family = AF_INET;
    // sin_addr.s_addr specifies the IP address of server. Specifies the network interface the server will bind to (e.g., localhost, specific IP, or all available interfaces). inet_addr() for a specific IP or INADDR_ANY() to bind to all interfaces. server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_addr.s_addr = inet_addr(IP);
    // server.sin_port is used to specify the port number where the server will listen
    server.sin_port = htons(PORT);

    // Binds the server socket to the specified IP and port.
    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        /*The bind function inhales file descriptor, server structure and size of server structure and returns a positive integer value if binding is successful*/
        perror("Bind failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully bound to port %d\n", PORT);

    // Listen for incoming connections
    /*
    -> Puts the server in passive mode, waiting for incoming connections.
    -> 5 specifies the maximum length of the queue for pending connections.*/
    if (listen(fd, 5) < 0)
    {
        perror("Listen failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening for connections...\n");

    // ---------------------------------------------------------------------------------------------------------------
    /* Accepts an incoming connection request and creates a new socket confd for communication with the client. */
    socklen_t clientLen = sizeof(client);
    confd = accept(fd, (struct sockaddr *)&client, &clientLen);
    if (confd < 0)
    {
        perror("Accept failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    /*
    inet_ntop: Converts the client's IP address to a readable string and stores it in str.
    ntohs: Converts the client's port number from network to host byte order. */
    printf(
        "Connection accepted from client [%s:%d]\n",
        inet_ntop(
            AF_INET,
            &client.sin_addr,
            str,
            INET_ADDRSTRLEN),
        ntohs(client.sin_port));

    // Get the current time and send it to the client
    ticks = time(NULL);
    snprintf(sendBuff, sizeof(sendBuff), "Current time: %.24s\r\n", ctime(&ticks));
    write(confd, sendBuff, strlen(sendBuff));
    printf("Sent time to client: %s", sendBuff);

    // Close connections
    close(confd);
    close(fd);

    return 0;
}
