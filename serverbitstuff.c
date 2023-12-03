#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int* bitDestuffing(const int* stuffedData, int n, int* resultSize)
{
    int* res = (int*)malloc(n * sizeof(int));
    if (res == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int res_index = 0;
    int i = 0;
    int count = 0;

    while (i < n)
    {
        if (stuffedData[i] == 0)
        {
            res[res_index++] = 0;
            i++;
        }
        else if (stuffedData[i] == 1)
        {
            res[res_index++] = 1;
            i++;
            count++;
            if (count == 5 && i < n && stuffedData[i] == 0)
            {
                i++;
                count = 0; // Reset the count
            }
        }
    }

    *resultSize = res_index;
    return res;
}

int main()
{
    char flag = '~', esc = '}';
    char buffer[MAX_BUFFER_SIZE];

    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, newAddr;
    socklen_t addrSize;
    ssize_t bytesRead;

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == 0)
    {
        printf("Listening...\n");
    }
    else
    {
        perror("Listening error");
        exit(EXIT_FAILURE);
    }

    addrSize = sizeof(newAddr);
    newSocket = accept(serverSocket, (struct sockaddr*)&newAddr, &addrSize); // Accept a client connection
    if (newSocket < 0)
    {
        perror("Acceptance error");
        exit(EXIT_FAILURE);
    }

    bytesRead = recv(newSocket, buffer, MAX_BUFFER_SIZE, 0); // Receive data from the client
    if (bytesRead < 0)
    {
        perror("Receiving error");
        exit(EXIT_FAILURE);
    }

    buffer[bytesRead] = '\0'; // Null-terminate the received data

    printf("Received stuffed data: %s\n", buffer);

    int receivedDataSize = strlen(buffer);
    int* receivedData = (int*)malloc(receivedDataSize * sizeof(int));
    if (receivedData == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < receivedDataSize; i++) {
        receivedData[i] = buffer[i] == '0' ? 0 : 1;
    }

    int resultSize;
    int* destuffedData = bitDestuffing(receivedData, receivedDataSize, &resultSize);

    printf("Destuffed Data: ");
    for (int i = 0; i < resultSize; i++) {
        printf("%d", destuffedData[i]);
    }
    printf("\n");

    close(newSocket);
    close(serverSocket);

    free(receivedData);
    free(destuffedData);

    return 0;
}

