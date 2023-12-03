#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"

char* byteStuffing(const char* str, char flag, char esc)
{
    int len = strlen(str);
    char* res = (char*)malloc((len * 2 + 3) * sizeof(char));
    if (res == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int res_index = 0;
    res[res_index++] = flag;

    for (int i = 0; i < len; i++)
    {
        if (str[i] == flag || str[i] == esc)
            res[res_index++] = esc;
        res[res_index++] = str[i];
    }

    res[res_index++] = flag;
    res[res_index] = '\0';
    
    return res;
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
    char str[100]; // Adjust the size as needed.

    printf("Enter the data in the form of a character string: ");
    scanf("%s", str);
    
    printf("Your data in binary is:");
    char *data=str;
    stringToBinary(data);

    char* res = byteStuffing(str, flag, esc);
    printf("\nThe data after Byte Stuffing is:");
    stringToBinary(res);

    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrSize;

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

    // Send the stuffed data to the server
    send(clientSocket, res, strlen(res), 0);
    
    
    printf("\n);


    close(clientSocket);

    free(res);

    return 0;
}

