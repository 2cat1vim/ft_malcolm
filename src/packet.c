#include "../include/malcolm.h"

#define SOURCE 0
#define TARGET 1
#define IPV4_L 4

int
crt_sock(t_malcolm *m) {
	int fd = -1;
	if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
		return (-1);

	if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, m->itrf, s_len(m->itrf)) == -1)
			return (-1);
	return (fd);
}

int
wfor_arp(t_malcolm *m) {
	unsigned char buf[42];
	struct sockaddr_ll from;
	socklen_t lenfrom = sizeof(from);
	ssize_t r = recvfrom(m->fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &lenfrom);
	if (r < 0)
		return (-1);
	t_ether_hdr *eth = (t_ether_hdr *)buf;
	t_arp_hdr *arp = (t_arp_hdr*)(buf + sizeof(t_ether_hdr));
	if (ntohs(arp->op) == ARPOP_REQUEST && ntohs(eth->type) == ETH_P_ARP) {
		if (cmp_mem(arp->tpa, &m->src_ip, IPV4_L) != 0)
			return (1);
		if (cmp_mem(arp->spa, &m->trg_ip, IPV4_L) != 0)
			return (1);
		printf("[🏷️ ]: An ARP request has been broadcast.\n"
			"	mac address of request: %02x:%02x:%02x:%02x:%02x:%02x\n"
			"	IP address of request: %hhu.%hhu.%hhu.%hhu\n", 
				arp->sha[0], arp->sha[1], arp->sha[2], 
				arp->sha[3], arp->sha[4], arp->sha[5],
				arp->spa[0], arp->spa[1], arp->spa[2],
				arp->spa[3]);
		return (0);
	}
	return (1);
}

void sto_arp(t_malcolm *m) {
	(void)m;
}
