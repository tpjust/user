#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{
int clientsocket,serversocket;
struct sockaddr_in serveraddress;
clientsocket=socket(AF_INET,SOCK_STREAM,0);
if (clientsocket<0)
printf("socket not formed\n");
else
printf("socket formed\n");
serveraddress.sin_family=AF_INET;
serveraddress.sin_port=htons(9000);
serveraddress.sin_addr.s_addr=INADDR_ANY;

int constatus=connect(clientsocket,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
if(constatus<0)
printf("not connected\n");
else
printf("connected\n");

char msg[256]="hello server";
send(clientsocket,msg,sizeof(msg),0);

close(clientsocket);
return 0;
}
