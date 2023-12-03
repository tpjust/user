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
#include <math.h>

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
	printf("-----------------------------------SLIDING WINDOW----------------------------------");
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

double min (double a, double b) {
	return a > b ? b : a;
}

int main () {

	int tp = 5;
	int tt = 1;
	int time = 0;


	printf("Enter tp (ms): ");
	fflush(stdout);
	scanf("%d%*c", &tp);
	printf("Enter tt (ms): ");
	fflush(stdout);
	scanf("%d%*c", &tt);


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


	int window_size = min(floor(log2(1 + 2*(tp/tt))), frame_count);

	printf("Window size is: %d", window_size);

	for (int i = 0; i < frame_count; i++) {
		char temp[1000];
		printf("\nEnter frame %d data: ", i);
		scanf("%[^\n]%*c", temp);
		printf("\nShould frame have error? (No - 0, Yes - 1): ");
		int error;
		scanf("%d%*c", &error);
		frames[i] = create_frame(temp, i, Seq, error ? true : false);
	}

	print_table_header();
	
	int sent = 0;
	int ackd = 0;
	int rem = frame_count;
	bool resending = false;
	print_table_data(time, sent, ackd, rem);


	int window_start = 0;
	int window_end = window_size - 1;



	int window_start_time = 0;


	while (ackd != frame_count) {
resend:
		for (int i = window_start; i <= window_end; i++) {
			Frame* frame = frames[i];


			time += tt;
			if (i == window_start) window_start_time = time;


			sent++;
			rem--;
			print_table_data(time, sent, ackd, rem);
			fflush(stdout);
			
			int send_status = sendto(client, frame, sizeof(Frame), 0, (struct sockaddr*)&address, sizeof(address));
			if (send_status < 0) {
				perror("\nError in sending.");
				exit(1);
			}
		}
		time = window_start_time + 2*tp;

	
		int failed_count = 0;
		int failed_start_time = time;

		for (int i = 0; i < window_size; i++) {
			Frame ack;
			int recv_status = recvfrom(client, &ack, sizeof(Frame), 0, (struct sockaddr*)&address, &address_length);
			if (recv_status < 0) {
				perror("\nError in receiving.");
				exit(1);
			}

			if (ack.type == PosAck) {
				ackd++;
				if (ackd == frame_count) {
					print_table_data(time, sent, ackd, rem);
					fflush(stdout);
					break;

				}
				window_start++;
				if (window_end + 1 < frame_count) window_end++;
			}

			print_table_data(time, sent, ackd, rem);
			fflush(stdout);
			time += tt;
		
			if (ack.type == NegAck) {

				if (failed_count == 0) {
					failed_start_time = time + tt;
				}

				frames[ack.seq_no - 1]->error = false;
				sent--;
				rem++;

				// send frame again
				sent++;
				rem--;

				failed_count++;
				print_table_data(time + tt, sent, ackd, rem);
				fflush(stdout);

				int send_status = sendto(client, frames[ack.seq_no - 1], sizeof(Frame), 0, (struct sockaddr*)&address, sizeof(address));
				if (send_status < 0) {
					perror("\nError in sending.");
					exit(1);
				}
				
			}
		}

		time = failed_start_time + 2 * tp;

		for (int i = 0; i < failed_count; i++) {
			Frame ack;
			int recv_status = recvfrom(client, &ack, sizeof(Frame), 0, (struct sockaddr*)&address, &address_length);
			if (recv_status < 0) {
				perror("\nError in receiving.");
				exit(1);
			}

			ackd++;
			window_start++;
			if (window_end + 1 < frame_count) window_end++;
			print_table_data(time, sent, ackd, rem);
			fflush(stdout);
			time += tt;
		}
	

		fflush(stdout);
	}


	// Cleanup
	close(client);
	return 0;
}