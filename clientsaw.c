#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define PORT 8888

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int packets;
    printf("Enter the number of packets: ");
    scanf("%d", &packets);

    double propagation_time, transmission_time;
    printf("Enter propagation time (ms): ");
    scanf("%lf", &propagation_time);
    printf("Enter transmission time (ms): ");
    scanf("%lf", &transmission_time);

    sendto(client_socket, &packets, sizeof(packets), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    double total_time = 0.0;
    for (int sent_packet = 0; sent_packet < packets; sent_packet++) {
        sendto(client_socket, &sent_packet, sizeof(sent_packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Simulate propagation and transmission time;

        int ack_packet;
        recvfrom(client_socket, &ack_packet, sizeof(ack_packet), 0, NULL, NULL);
        
        total_time += 2*propagation_time + transmission_time;   
        usleep((useconds_t)(total_time*1e3));
        printf("Received ACK for packet: %d\n", ack_packet);

        // Accumulate the total time
        printf("Time: %.6fms\n", total_time);
    }

    printf("Total time taken: %.6f ms\n", total_time);
    printf("Total number of transmissions: %d\n", packets * 2); // Assuming every packet requires 2 transmissions (send and receive)

    close(client_socket);
    return 0;
}

