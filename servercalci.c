#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

double calculate(double num1, double num2, char operator) {
    switch (operator) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 != 0)
                return num1 / num2;
            else
                return 0.0; // Division by zero error
        default:
            return 0.0; // Invalid operator
    }
}

int main()
{
int serversocket,client_socket,bindstatus,a,b,valread,sum=0;
char buffer[BUFFER_SIZE] = {0};
char buffer1[BUFFER_SIZE] = {0};
char buffer2[BUFFER_SIZE] = {0};
char buffer3[BUFFER_SIZE] = {0};
char servermessage[256]="Hello Client and we are connected now";
struct sockaddr_in serveraddress;


serversocket=socket(AF_INET,SOCK_STREAM,0);
if(serversocket<0)
printf("socket failed");
serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
//serveraddress.sin_addr.s_addr= htons("192.168.56.1");
serveraddress.sin_addr.s_addr= INADDR_ANY;

bindstatus=bind(serversocket,(struct sockaddr *)&serveraddress, sizeof(serveraddress));
if(bindstatus<0)
printf("binding failed\n");
else
printf("binding is successful\n");

listen(serversocket,3);
printf("Send reply to the client\n");

//clientsocket=accept(serversocket,( struct sockaddr *)&clientsocket, sizeof(clientsocket));
client_socket=accept(serversocket,NULL,NULL);
if(client_socket<0)
printf("connection is rejected by server\n");
else
printf("connection is accepted by server\n");
printf("\n");


// Read data from client
 //   valread = read(client_socket, buffer, BUFFER_SIZE);
 //   printf("Received expression from client: %s\n", buffer);
    recv(client_socket, buffer1, BUFFER_SIZE,0);
    printf("Received expression from client: %s\n", buffer1);
    recv(client_socket, buffer2, BUFFER_SIZE,0);
    printf("Received expression from client: %s\n", buffer2);
    recv(client_socket, buffer3, BUFFER_SIZE,0);
    printf("Received expression from client: %s\n", buffer3);

    // Parse the expression
    double num1, num2, result;
    char operator;
//    sscanf(buffer, "%lf%c%lf", &num1, &operator, &num2);
    sscanf(buffer1, "%lf", &num1);
    sscanf(buffer2, "%lf", &num2);
    sscanf(buffer3, "%s", &operator);


    // Perform the calculation
    result = calculate(num1, num2, operator);

    // Convert the result to string
    snprintf(buffer, BUFFER_SIZE, "%.2lf", result);

    // Send the result to client
    send(client_socket, buffer, strlen(buffer), 0);
    printf("Result sent to client: %s\n", buffer);
    
close(client_socket);
close(serversocket);
return 0;

}

