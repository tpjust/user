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
	printf(" Time (tt)    |    Message");
	printf("\n");
	printf("-----------------------------------------------------------------------------------");
	printf("\n");
	fflush(stdout);
}

void print_table_data (int time, char* x, int frame_num, char* message) {
	printf(" %4d (tt)    |    %s%d - %s", time, x, frame_num, message);
	printf("\n");
	fflush(stdout);
}

void print_table_data2 (int time, char* x, int frame_num) {
	printf(" %4d (tt)    |    %s%d", time, x, frame_num);
	printf("\n");
	fflush(stdout);
}

int main () {

	int tp = 5;
	int tt = 1;



	int server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int enable = 1;
	setsockopt(server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable));


	Address address;
	address.sin_port = htons(8080);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_family = AF_INET;
	socklen_t address_length = sizeof(address);
	bind(server, (struct sockaddr*)&address, sizeof(Address));
	print_table_header();

	int time = 0; 
	bool resending = false;


	while (1) {
		Frame data;

		// Receive frame
		int recv_status = recvfrom(server, &data, sizeof(Frame), 0, (struct sockaddr*)&address, &address_length);
		if (recv_status < 0) {
			perror("\nError in sending.");
			exit(1);
		}
		// printf("\n\nReceived data: %s", data.packet.data);	
		time += (tp);
		if (!resending) {
			time += tt;
		}
		print_table_data(time, "Received frame ", data.seq_no, data.packet.data);
		fflush(stdout);

		Frame* ack = create_frame(data.packet.data, data.seq_no + 1, data.error ? NegAck : PosAck, false);

		// Send acknowledgement
		int send_status = sendto(server, ack, sizeof(Frame), 0, (struct sockaddr*)&address, sizeof(address));
		if (send_status < 0) {
			perror("\nError in sending.");
			exit(1);
		}
		if (ack->type == PosAck) {
			print_table_data2(time, "Sent Positive Acknowledgement for frame", data.seq_no);
			resending = false;
		}
		else {
			print_table_data2(time, "Sent Negative Acknowledgement for frame", data.seq_no);
			resending = true;
		}
		time += tp;

	}
	

	// Cleanup
	close(server);
	return 0;
}