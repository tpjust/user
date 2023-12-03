#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
//create Socket
int client_socket,a,b,sum,valread;
    char buffer[BUFFER_SIZE];
    char firstnum[BUFFER_SIZE];    
    char secondnum[BUFFER_SIZE];
    char op[BUFFER_SIZE];
//char serverresponse[256];
struct sockaddr_in serveraddress;

// printf("Enter the expression : ");
//    fgets(expression, BUFFER_SIZE, stdin);
//    expression[strcspn(expression, "\n")] = '\0'; // Remove the newline character

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

 //char serverresponse[256];
 //int len=sizeof(serverresponse);
 //recv(client_socket,&serverresponse,len,0);
 
 //printf("The reply from the server is:%s\n",serverresponse);
 // Send expression to server
  //  send(client_socket, expression, strlen(expression), 0);
 //   printf("Expression sent to server: %s\n", expression);
   printf("Enter first number : ");
   scanf("%s",&firstnum);
      send(client_socket, firstnum, strlen(firstnum), 0);
      printf("Expression sent to server: %s\n", firstnum);
   printf("Enter the esecond number : ");
   scanf("%s",&secondnum);
   
      send(client_socket, secondnum, strlen(secondnum), 0);
      printf("Expression sent to server: %s\n", secondnum);
   printf("Enter the operator : ");
   scanf("%s",&op);
      send(client_socket, op, strlen(op), 0);
      printf("Expression sent to server: %s\n", op);

    // Receive result from server
    recv(client_socket, buffer, BUFFER_SIZE,0);
    printf("Received result from server: %s\n", buffer);
 
 close(client_socket);
 
 return 0;
 
 }
 
