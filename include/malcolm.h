#ifndef MALCOLM_H
#define MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <stdbool.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <errno.h>
#include <net/if_arp.h>

#include "../lib/lib.h"

typedef struct s_malcolm {
	int fd;
	char *itrf;
	uint8_t src_ip[4];
	uint8_t trg_ip[4];
	uint8_t src_mac[6];
	uint8_t trg_mac[6];
} t_malcolm;
/*    Ethernet packet data:
        16.bit: (ar$hrd) Hardware address space (e.g., Ethernet,
                         Packet Radio Net.)
        16.bit: (ar$pro) Protocol address space.  For Ethernet
                         hardware, this is from the set of type
                         fields ether_typ$<protocol>.
         8.bit: (ar$hln) byte length of each hardware address
         8.bit: (ar$pln) byte length of each protocol address
        16.bit: (ar$op)  opcode (ares_op$REQUEST | ares_op$REPLY)
        nbytes: (ar$sha) Hardware address of sender of this
                         packet, n from the ar$hln field.
        mbytes: (ar$spa) Protocol address of sender of this
                         packet, m from the ar$pln field.
        nbytes: (ar$tha) Hardware address of target of this
                         packet (if knowHn).
        mbytes: (ar$tpa) Protocol address of target. */

typedef struct __attribute__((packed)) {
	uint16_t hrd;
	uint16_t pro;
	uint8_t hln;
	uint8_t pln;
	uint16_t op;
	uint8_t sha[6];
	uint8_t spa[4];
	uint8_t tha[6];
	uint8_t tpa[4];
} t_arp_hdr;

/*DMAC
6 bytes
Destination MAC address. Six bytes for IPv4. This field identifies the receiver of a frame.
SMAC
6 bytes
Source MAC address. Six bytes for IPv4. This field identifies the sender of a frame.
Type
2 bytes
Protocol type. Table 1-3 lists the types of protocols that are directly encapsulated on a link.*/
typedef struct __attribute__((packed)) {
	uint8_t dmac[6];
	uint8_t smac[6];
	uint16_t type;
} t_ether_hdr;

void init_malcolm(t_malcolm *m);
void free_malcolm(void *s_);
bool parse_point(t_malcolm *m, char **av);
int crt_sock(t_malcolm *m);
int wfor_arp(t_malcolm *m);
void sto_arp(t_malcolm *m);
#endif
