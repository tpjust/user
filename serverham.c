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
int serversocket,client_socket,bindstatus,valread;
char buffer[BUFFER_SIZE] = {0};
    int hammingCode[11];
    int p1, p2, p4, p8;

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
    valread = read(client_socket, buffer, BUFFER_SIZE);
    printf("Received expression from client: %s\n", buffer);
    int i=0;
    
        for (i = 0; i < 11; i++)
    {
        hammingCode[i] =buffer[i] ;
    }
    
    //recieved data checking
    void checkParityBits(int hammingCode[10], int *p1, int *p2, int *p4, int *p8) {
    *p1 = hammingCode[10]^hammingCode[0] ^ hammingCode[2] ^ hammingCode[4] ^ hammingCode[6] ^ hammingCode[8];
    *p2 = hammingCode[9]^hammingCode[0] ^ hammingCode[1] ^ hammingCode[4] ^ hammingCode[5] ^ hammingCode[8];
    *p4 = hammingCode[7]^hammingCode[6] ^ hammingCode[5] ^ hammingCode[4];
    *p8 = hammingCode[3]^hammingCode[0] ^ hammingCode[1] ^ hammingCode[2];
}

    checkParityBits(hammingCode, &p1, &p2, &p4, &p8);
    printf("Parity (p8 p4 p2 p1)=%d%d%d%d\n",p8,p4,p2,p1);
    
    if (p1 == 0 && p2 == 0 && p4 == 0 && p8 == 0) {
        printf("Accepted\n");
    } else {
        printf("Not Accepted\n");
    }
    
close(client_socket);
close(serversocket);
return 0;

}

    
    
    
    
    

