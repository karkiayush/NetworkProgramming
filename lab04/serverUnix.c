#include <stdio.h>      // For input/output functions
#include <stdlib.h>     // For memory management and exit()
#include <string.h>     // For string operations (e.g., memset, strncpy)
#include <unistd.h>     // For close() and unlink()
#include <sys/socket.h> // For socket programming
#include <sys/un.h>     // For UNIX domain socket structures
#include <errno.h>      // For error reporting
#include <sys/types.h>  // For various data types like ssize_t

/* The PATH in this program refers to a file system path where the UNIX domain socket file of server will be created. */
// #define PATH "/online/mxpfo/trace/sarod/un.txt"
#define PATH "/home/aayush-krk/Programming/NetworkProgramming/unix_socket"

int main()
{
    // Step 1: Create a UNIX domain socket
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("Error creating socket %d\n", fd);
        exit(-1); // Exit if socket creation fails
    }

    // Step 2: Configure server address structure
    struct sockaddr_un server, client;
    memset(&server, 0, sizeof(struct sockaddr_un));              // Initialize server struct
    server.sun_family = AF_UNIX;                                 // Set address family to UNIX
    strncpy(server.sun_path, PATH, sizeof(server.sun_path) - 1); // Assign socket file path
    unlink(PATH);                                                // Remove any existing socket file at PATH

    // Step 3: Bind the socket to the file path
    if (bind(fd, (struct sockaddr *)&server, sizeof(struct sockaddr_un)) == 0)
    {
        printf("Bind successful with File Path[%s] and socket[%d]\n", PATH, fd);
    }

    // Step 4: Put the server in listening mode
    int ls = listen(fd, 5);
    printf("Listen return value[%d]\n", ls);
    if (ls == -1)
    {
        printf("Error on Listening\n");
        exit(-1); // Exit if listening fails
    }
    else
        printf("System is Listening on SOCKET[%s]\n", PATH);

    // Step 5: Accept an incoming client connection
    size_t len = sizeof(client);
    int confd = accept(fd, (struct sockaddr *)&client, &len);
    printf("Connected Socket Descriptor[%d]\n", confd);

    // Step 6: Exchange messages with the client
    char recvBuff[128];
    char sendBuff[128] = "Hello this is UNIX Server";       // Response message
    ssize_t r = recv(confd, recvBuff, sizeof(recvBuff), 0); // Receive data from client
    printf("Message received from UNIX-Client[%s], msglen[%ld]\n", recvBuff, r);
    ssize_t s = send(confd, sendBuff, 100, 0); // Send response to client
    printf("Message sent to UNIX-Client[%s], msg len[%ld]\n", sendBuff, s);

    // Step 7: Cleanup resources
    close(confd); // Close the client socket
    close(fd);    // Close the server socket
    unlink(PATH); // Remove the socket file

    return 0; // End of program
}
