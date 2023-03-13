#include "Sim_Engine.h"
#include <string.h>

int checksum(char *p, int ack, int seq);

struct pkt make_pkt(int seq, char *p);
