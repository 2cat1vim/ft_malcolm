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
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#include "../lib/lib.h"

typedef struct s_malcolm {
	char *itrf;
	struct in_addr ips[2];
	struct ether_addr macs[2];
} t_malcolm;

void init_malcolm(t_malcolm *m);
void free_malcolm(void *s_);
bool parse_point(t_malcolm *m, char **av);
#endif
