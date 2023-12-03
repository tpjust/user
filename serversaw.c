#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8888

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server waiting for incoming connections...\n");


    int expected_packet = 0;
         int received_packet,total_packets;
         recvfrom(server_socket, &total_packets, sizeof(total_packets), 0, (struct sockaddr *)&client_addr, &addr_len);
         printf("Total number of packets to be received: %d\n", total_packets);

        for (int i = 0; i < total_packets; i++) {
        recvfrom(server_socket, &received_packet, sizeof(received_packet), 0, (struct sockaddr *)&client_addr, &addr_len);

        if (received_packet == expected_packet) {
            printf("Received packet: %d\n", received_packet);
            sendto(server_socket, &received_packet, sizeof(received_packet), 0, (struct sockaddr *)&client_addr, addr_len);
            expected_packet += 1; 
           }
        }

    close(server_socket);
    return 0;
}

