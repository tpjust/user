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
int serversocket,client_socket,bindstatus,valread;
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
char buffer[1024];
    ssize_t bytes_received;
    
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        send(client_socket, buffer, bytes_received, 0);
    }
    
    printf("Connection closed\n");
    
    // Close sockets
    close(client_socket);
    close(serversocket);
    
    return 0;
}
