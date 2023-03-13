#include "Sim_Engine.h"
#include "common.h"
#include <string.h>
#include <stdbool.h>
#define BUF_SIZE 100000
static float TIME_OUT = 10.0;
static int size = 0;
static struct pkt curr_packet;
//struct msg buf[BUF_SIZE];
static char* buf[BUF_SIZE];
static struct msg curr_message;
static int i = 0;
static int x = 0;
static int one_or_zero;
static bool sending;
/* Called from layer 5, passed the data to be sent to other side */
void A_output( struct msg message) {
  /* TODO */
	//Store messages in buffer	
	buf[i] = message.data;
	//If not sending, store more messages and send the first one
	if(!sending) {
	struct pkt toSend = make_pkt(one_or_zero, buf[i]);
	i++;
	curr_packet = toSend;
	if(one_or_zero == 0)
		one_or_zero = 1;
	else
		one_or_zero = 0;
	sending = true;
	tolayer3(0, toSend);	
	starttimer(0, TIME_OUT);
	}

}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
  /* TODO */
	stoptimer(0);
	//If corrupt, send back
	if(packet.acknum != packet.seqnum || packet.checksum != curr_packet.checksum) {
		printf("Packet corrupted in A\n");	
		tolayer3(0, curr_packet);
		starttimer(0, TIME_OUT);
	}
	else {
		printf("Packet recieved correctly\n");
		sending = false;
		//If there are messages in the buffer send the next one
		if(buf[i]) {
			sending = true;
			if(one_or_zero == 0)
				one_or_zero = 1;
			else
				one_or_zero = 0;
			
			struct pkt sendPkt = make_pkt(one_or_zero, buf[i]);
			curr_packet = sendPkt;
			tolayer3(0, sendPkt);
			starttimer(0, TIME_OUT);
			i++;
		}
	}
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  /* TODO */
	//If timeout, resend package
	printf("TimeOut... RESENDING\n");
	tolayer3(0, curr_packet);
	starttimer(0, TIME_OUT);
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  /* TODO */
	one_or_zero = 0;
	sending = false;
}
