#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_DIGITS 15

// Function to calculate the parity bits for the Hamming code
void calculateParityBits(int data[7], int *p1, int *p2, int *p4, int *p8) {
    *p1 = data[0] ^ data[2] ^ data[3] ^ data[5] ^ data[6];
    *p2 = data[0] ^ data[1] ^ data[3] ^ data[4] ^ data[6];
    *p4 = data[5] ^ data[4] ^ data[3];
    *p8 = data[0] ^ data[1] ^ data[2];
}

void replaceLetter(char *buffer, int position, char newLetter) {
    if (position >= 0 && position < strlen(buffer)) {
        buffer[position] = newLetter;
    } else {
        printf("Invalid position! The position must be within the range of the string.\n");
    }
}


int main()
{
//create Socket
int client_socket,position,choice;
char buffer[BUFFER_SIZE] = {0},newLetter;
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

    int data[7],i;
    int hammingCode[11] = {0};
    int p1, p2, p4, p8;

    printf("Enter 7 bits of data: ");
    for (i = 0; i < 7; i++) {
        scanf("%1d", &data[i]);
    }
    // Calculate parity bits and construct the Hamming code
    calculateParityBits(data, &p1, &p2, &p4, &p8);
    hammingCode[0] = data[0];
    hammingCode[1] = data[1];
    hammingCode[2] = data[2];
    hammingCode[4] = data[3];
    hammingCode[5] = data[4];
    hammingCode[6] = data[5];
    hammingCode[8] = data[6];
    hammingCode[3] = p8;
    hammingCode[7] = p4;
    hammingCode[9] = p2;
    hammingCode[10] = p1;

    // Print the Hamming code
    printf("Hamming Code: ");
    for (i = 0; i < 11; i++) {
        printf("%d", hammingCode[i]);
    }
    printf("\n");
    
    //convert array to string
       char buff[MAX_DIGITS + 1]; // Add 1 for the null terminator

    // Clear the result_string buffer
    buffer[0] = '\0';

    for (i = 0; i < 11; i++) {
        // Convert the current number to a string
        snprintf(buff, MAX_DIGITS + 1, "%d", hammingCode[i]);

        // Concatenate the current number to the result_string
        strcat(buffer, buff);

    }
    
    printf("do you want to put an error in data \n1.yes\n0.no\n");
    scanf("%d",&choice);
    if(choice)
    {
        printf("enter the position you want to replace=");
        scanf("%d",&position);
        getchar();
            printf("Enter the new bit: ");
            scanf("%c", &newLetter);

            replaceLetter(buffer, position-1, newLetter);

    }

   send(client_socket, buffer, strlen(buffer), 0);
   printf("Expression sent to server: %s\n", buffer);
   
 close(client_socket);
 
 return 0;
 
 }
 
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
