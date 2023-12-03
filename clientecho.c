#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
int main()
{
//create Socket
int client_socket;
struct sockaddr_in serveraddress;

client_socket=socket(AF_INET,SOCK_STREAM,0);// create socket
//SOCKET function returns the integer
if(client_socket<0)
printf("socket is not created\n");
else 
printf("socket is created succesfully\n");

serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
//serveraddress.sin_addr.s_addr= htons("192.168.56.1");
serveraddress.sin_addr.s_addr= INADDR_ANY;

int constatus=connect(client_socket,(struct sockaddr *) &serveraddress, sizeof(serveraddress));
 
 if(constatus==-1)
 printf("there was an error in the connection\n");
 else 
 printf("connection is estableshed\n"); 
 
     char message[1024];
    printf("Enter a message to send: ");
    fgets(message, sizeof(message), stdin);
    
    send(client_socket, message, strlen(message), 0);
    
    // Receive and print echoed message
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        printf("Received: %.*s\n", (int)bytes_received, buffer);
    }
    
     close(client_socket);
 
 return 0;
 
 }
