#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server socket configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Use any available port
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    // Accept incoming connection
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data from the client and verify byte count
        int byte_count = 0;
        // Read the byte count field
        read(client_socket, &byte_count, sizeof(int));

        printf("ByteCount: %d\n", byte_count);
        // Read the message based on the byte count
        read(client_socket, buffer, byte_count);

        // Verify byte count
        if (strlen(buffer) != byte_count) {
            printf("Error: Byte count mismatch\n");
        }
        else{
            printf("Byte count matched\n");
        }
        

        printf("Received from client: %s\n", buffer);
        

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}

