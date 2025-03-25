#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <asm-generic/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Client Code
void client()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    // Connect to the server
    if (
        connect(
            sock,
            (struct sockaddr *)&serv_addr,
            sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    // Send data to the server
    send(sock, "Hello from client", strlen("Hello from client"), 0);

    // Read the response from the server
    read(sock, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);

    close(sock);
}

int main()
{
    // Run server in one terminal, client in another
    // For simplicity, we'll just run client here
    client();
    return 0;
}