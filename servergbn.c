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

double min (double a, double b) {
	return a > b ? b : a;
}


int main () {

	int tp = 5;
	int tt = 1;
	int frame_count = 0;

	printf("Enter tp (ms): ");
	fflush(stdout);
	scanf("%d%*c", &tp);
	printf("Enter sender tt (ms): ");
	fflush(stdout);
	scanf("%d%*c", &tt);


	printf("\nEnter frame count: ");
	scanf("%d%*c", &frame_count);


	int window_size = min(floor(log2(1 + 2*(tp/tt))), frame_count);

	printf("\nWindow size is: %d\n", window_size);


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
	
	int window_progress = 0;
	int last_successful_frame = -1;


	time += tp;

	while (1) {
		Frame data;

		if (window_progress == window_size) {
			time += 2*tp + tt;
			window_progress = 0;
		}
		else {
			time += tt;
		}

		// Receive frame
		int recv_status = recvfrom(server, &data, sizeof(Frame), 0, (struct sockaddr*)&address, &address_length);
		window_progress++;
		if (recv_status < 0) {
			perror("\nError in sending.");
			exit(1);
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

			if (resending && data.seq_no != last_successful_frame + 1) {
				print_table_data2(time, "Discarding frame", data.seq_no);
			}
			else {
				last_successful_frame++;
				print_table_data2(time, "Sent Positive Acknowledgement for frame", data.seq_no);
				resending = false;
			}

		}
		else {
			print_table_data2(time, "Sent Negative Acknowledgement for frame", data.seq_no);
			resending = true;
		}

	}
	

	// Cleanup
	close(server);
	return 0;
}