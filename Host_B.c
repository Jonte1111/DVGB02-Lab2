#include "Sim_Engine.h"
#include "common.h"
#include <stdbool.h>
#include <string.h>
int expectedPkt;
struct msg message;
int test = 0;
/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
  /* TODO */
	printf("Packet recieved at B\n");
	//Check for correct package, if last was 0 this should be 1
	//most likely same package was sent twice
	if(packet.seqnum != expectedPkt) {
		printf("Double");
		tolayer3(1, packet);
	}
	//Calculate checksum of package and store it
	int sum = checksum(packet.payload, packet.acknum, packet.seqnum);
	printf("SUM: %d, CHECK: %d\n", sum, packet.checksum);
	printf("SEQ: %d, EXP: %d\n", packet.seqnum, expectedPkt);
	//If checksum and seqnum is correct send ACK
	if((sum == packet.checksum) && (packet.seqnum == expectedPkt)) {
		printf("Sending ACK%d\n", expectedPkt);
		if(expectedPkt == 0)
			expectedPkt = 1;
		else
			expectedPkt = 0;
		//Send to layer 3 and 5
		tolayer5(1, packet.payload);
		tolayer3(1, packet);
	}
	//ACK -> seqnum == acknum, NACK -> seqnum != acknum
	else {
		printf("%d || %d\n", packet.acknum, packet.seqnum);
		int nak;
		if(expectedPkt == 1)
			nak = 0;
		else
			nak = 1;
		packet.acknum = nak;
		tolayer3(1, packet);
	}
}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
  /* TODO */
	expectedPkt = 0;
}
