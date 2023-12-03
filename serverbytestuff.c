#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

char* byteDestuffing(const char* stuffedData, char flag, char esc)
{
    int len = strlen(stuffedData);
    char* destuffed = (char*)malloc(len * sizeof(char));
    if (destuffed == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int destuffIndex = 0;
    int i = 0;


    while (i < len - 1) { // Stop before the last flag
        if (stuffedData[i] == flag) {
            if (stuffedData[i + 1] == flag) {
                destuffed[destuffIndex++] = flag;
                i += 2;
            } else{
                destuffed[destuffIndex++] = esc;
                i += 2;
            } 
        } else {
            destuffed[destuffIndex++] = stuffedData[i++];
        }
    }


    destuffed[destuffIndex] = '\0';
    
    return destuffed;
}


void stringToBinary(char *str)
{
    int len = strlen(str);
    
    for (int i = 0; i < len; i++)
    {
        char c = str[i];
        for (int j = 7; j >= 0; j--)
        {
            int bit = (c >> j) & 1;
            printf("%d", bit);
        }
        printf(" "); 
    }
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

    buffer[bytesRead] = '\0'; // Null-terminate the received data

    printf("Received stuffed data: ");
    char *data=buffer;
    stringToBinary(data);
    


    char* destuffedData = byteDestuffing(buffer, flag, esc);
    printf("\nDestuffed Data:");

    char *data1=destuffedData;
    stringToBinary(data1);
    
    printf("\n");

    
    close(newSocket);
    close(serverSocket);

    free(destuffedData);

    return 0;
}

