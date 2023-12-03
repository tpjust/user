#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>

int main()
{
int clientsocket,serversocket;
struct sockaddr_in serveraddress;

serversocket=socket(AF_INET,SOCK_STREAM,0);
if(serversocket<0)
printf("socket failed");
serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
//serveraddress.sin_addr.s_addr= htons("192.168.56.1");
serveraddress.sin_addr.s_addr= INADDR_ANY;

int bindstatus=bind(serversocket,(struct sockaddr *)&serveraddress, sizeof(serveraddress));
if(bindstatus<0)
printf("binding failed\n");
else
printf("binding is successful\n");

listen(serversocket,3);
printf("Send reply to the client\n");

//clientsocket=accept(serversocket,( struct sockaddr *)&clientsocket, sizeof(clientsocket));
clientsocket=accept(serversocket,NULL,NULL);
if(clientsocket<0)
printf("connection is rejected by server\n");
else
printf("connection is accepted by server\n");
printf("\n");

char msg[256];
recv(clientsocket,msg,sizeof(msg),0);
printf("%s",msg);

close(serversocket);
return 0;
}
