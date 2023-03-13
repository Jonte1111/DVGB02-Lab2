#include "common.h"
//Calculate checksum using message, acknum and seqnum
int checksum(char *p, int ack, int seq) {
	int checksum = 0;
	for(int i = 0; i < 20; i++) 
		checksum += p[i];
	checksum += ack; 
	checksum += seq;
	return checksum;

}
//Create a new package
struct pkt make_pkt(int seq, char *p) {
	struct pkt newPkt;	
	newPkt.seqnum = seq;
	newPkt.acknum = seq;
	strcpy(newPkt.payload, p);
	int sum = checksum(newPkt.payload, newPkt.acknum, newPkt.seqnum);
	newPkt.checksum = sum;
	return newPkt;
}
