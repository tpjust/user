#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

int* bitStuffing(const int* arr, int n, int* resultSize)
{
    int* res = (int*)malloc(n * 2 * sizeof(int));
    if (res == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int res_index = 0;
    int i = 0;

    while (i < n)
    {
        while (i < n && arr[i] == 0)
        {
            res[res_index++] = 0;
            i++;
        }

        int count = 0;
        while (i < n && arr[i] == 1 && count < 5)
        {
            res[res_index++] = 1;
            i++;
            count++;
        }

        if (count == 5)
        {
            res[res_index++] = 0;
        }
    }

    *resultSize = res_index;
    return res;
}

char* binaryArrayToBinaryString(const int* binaryArray, int arraySize)
{
    char* binaryString = (char*)malloc((arraySize + 1) * sizeof(char));
    if (binaryString == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < arraySize; i++) {
        binaryString[i] = binaryArray[i] == 0 ? '0' : '1';
    }
    binaryString[arraySize] = '\0';

    return binaryString;
}

int main()
{
    int n;
    printf("Enter the size of data: ");
    scanf("%d", &n);

    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    printf("Enter the binary number with spaces:\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    int resultSize;
    int* res = bitStuffing(arr, n, &resultSize);

    char* binaryOutputString = binaryArrayToBinaryString(res, resultSize);
    printf("\nBinary output string: %s\n", binaryOutputString);

    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Connection error");
        exit(EXIT_FAILURE);
    }

    // Send the binary
        send(clientSocket, binaryOutputString, strlen(binaryOutputString), 0);
    

    close(clientSocket);
    
    free(res);
    return 0;
}

