#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#define PORT 9999             // Port number of the server
#define SERVER_IP "10.0.2.15" // IP address of the server

int main()
{
    int sockfd;                    // Socket file descriptor
    struct sockaddr_in serverAddr; // Server address structure
    char recvBuff[1024];           // Buffer to store data received from the server

    printf("\nWelcome to TCP Time Client\n");

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created with descriptor [%d]\n", sockfd);

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;   // IPv4
    serverAddr.sin_port = htons(PORT); // Port
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        perror("Invalid address or address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Connection to the server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server [%s:%d]\n", SERVER_IP, PORT);

    // Receive the current time from the server
    memset(recvBuff, 0, sizeof(recvBuff));
    int bytesRead = read(sockfd, recvBuff, sizeof(recvBuff) - 1);
    if (bytesRead < 0)
    {
        perror("Failed to read from server");
    }
    else
    {
        printf("Server time: %s", recvBuff);
    }

    // Close the connection
    close(sockfd);
    printf("Connection closed\n");

    return 0;
}

/*
tcp udp socket call
tcp udp diagram
tcp udp socket address_incall
bind call
*/
