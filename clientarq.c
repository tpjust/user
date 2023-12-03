// Socket communication libraries
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Standard Libraries
#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct sockaddr_in Address;


typedef enum {
	Seq = 0, PosAck = 1, NegAck = 2
} FrameType;

typedef struct {
	char data[1000];
} Packet;

typedef struct {
	int seq_no;
	Packet packet;
	FrameType type;
	bool error;
} Frame;

Frame* create_frame (char* data, int seq_no, FrameType type, bool error) {
	Frame* frame = (Frame*)malloc(sizeof(Frame));
	strcpy(frame->packet.data, data);
	frame->seq_no = seq_no;
	frame->type = type;
	frame->error = error;
	return frame;
}


void print_table_header() {
	printf("-----------------------------------STOP AND WAIT-----------------------------------");
	printf("\n");
	printf(" Time (tt)    |   Sent   |   Acknowledged   |   Remaining                          ");
	printf("\n");
	printf("-----------------------------------------------------------------------------------");
	printf("\n");
	fflush(stdout);
}


void print_table_data (int time, int sent, int acknowledged, int remaining) {
	printf(" %4d (ms)    |%7d   |%15d   |%12d", time, sent, acknowledged, remaining);
	printf("\n");
	fflush(stdout);
}

int main () {

	clock_t prev_time = clock(); 
	clock_t current_time = 0; 
	int tp = 5;
	int tt = 1;
	int time = 0;



	int client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int enable = 1;
	setsockopt(client, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable));

	Address address;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	socklen_t address_length = sizeof(address);

	

	int frame_count = 0;
	int current_frame = 0;
	printf("\nEnter frame count: ");
	scanf("%d%*c", &frame_count);
	Frame* frames[frame_count];

	for (int i = 0; i < frame_count; i++) {
		char temp[1000];
		printf("\nEnter frame %d data: ", i);
		scanf("%[^\n]%*c", temp);
		printf("\nShould frame have error? (No - 0, Yes - 1): ");
		int error;
		scanf("%d%*c", &error);
		frames[i] = create_frame(temp, i, Seq, error == 1 ? true : false);
	}

	print_table_header();
	
	int sent = 0;
	int ackd = 0;
	int rem = frame_count;
	bool resending = false;
	print_table_data(time, sent, ackd, rem);

	
	while (frame_count != 0) {
	

		// Send the frame
		if (!resending) {
			time += tt;
		sent++;
		rem--;
		}
		print_table_data(time, sent, ackd, rem);
		int send_status = sendto(client, frames[current_frame], sizeof(Frame), 0, (struct sockaddr*)&address, sizeof(address));
		if (send_status < 0) {
			perror("\nError in sending.");
			exit(1);
		}
		time += tp;

		// Wait for acknowledgement
		Frame ack;
		int recv_status = recvfrom(client, &ack, sizeof(Frame), 0, (struct sockaddr*)&address, &address_length);
		if (recv_status < 0) {
			perror("\nError in sending.");
			exit(1);
		}
		
		time += (tp);
		if (ack.type == PosAck) {
			ackd++;
			resending = false;
		}
		else {
			frames[current_frame]->error = false;
			resending = true;
		}



		print_table_data(time, sent, ackd, rem);
		fflush(stdout);

		if (ack.type == PosAck) {
			current_frame++;
			frame_count--;
		}
		
		fflush(stdout);
	}


	// Cleanup
	close(client);
	return 0;
}