/* Unix domain socket client code written in C.
Unix domain socket allows for inter-process communication(IPC) on the same host.
*/
#include <stdio.h>      // for standart i/o operation
#include <stdlib.h>     // for memory management
#include <string.h>     // for string operation
#include <unistd.h>     // for access to the close fun
#include <sys/socket.h> //
#include <sys/un.h>
#include <errno.h>
#define PATH "/home/aayush-krk/Programming/NetworkProgramming/unix_socket" // define the path for unix domain socket file

int main(int arg, char **argv)
{
        // socket creation
        int fd;
        // AF_UNIX specifies the address family for unix domain socket
        fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (fd > 0)
        {
                printf("ENd point created\n");
        }

        /* while communicating with the unix domain socket, it can does the communication within the same unix system only. */
        struct sockaddr_un client; // used to store the address of unix domain socket(address of client in this case)
        memset(&client, 0, sizeof(struct sockaddr_un));
        // Below code set the family of client to AF_UNIX(for unix domain socket)
        client.sun_family = AF_UNIX;
        // client.sun_path specifies destination path to the socket file
        strncpy(client.sun_path, PATH, sizeof(client.sun_path) - 1);

        /* ------------------Connecting to the server-------------- */
        // connect(): Attempts to connect the client socket to the server socket located at PATH.
        int c = connect(fd, (struct sockaddr *)&client, sizeof(struct sockaddr_un));
        if (c == -1)
        {
                printf("Error in Connecting to server[%d]\n", errno);
        }

        /* ----------------Sending a message to the server------------- */
        const char *sendmsg = "Hello this is UNIX Domain Client";
        ssize_t s = send(fd, sendmsg, strlen(sendmsg), 0);
        printf("Message send to  UNIX-Server[%s]msg len[%d]\n", sendmsg, s);

        /*-------------Receiving a response from server------------ */
        char recvBuff[100];
        // client waits for a response from the server using recv()
        ssize_t r = recv(fd, recvBuff, 100, 0);
        printf("Message Received from  UNIX-Server[%s]msg len[%d]\n", recvBuff, r);
        // closing the socket
        close(fd);
        return 0;
}

/* What the Code Does
1. Creates a UNIX domain socket client.
2. Connects to a server socket at the file path /online/mxpfo/trace/sarod/un.txt.
3. Sends a message to the server: "Hello this is UNIX DOmain Client".
4. Waits for a response from the server and prints it.
5. Closes the connection. */