#include "../include/malcolm.h"

int
crt_sock(t_malcolm *m) {
	int fd = -1;
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1)
		return (-1);

	if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, m->itrf, s_len(m->itrf)) == -1)
			return (-1);

	if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1)
		return (-1);
	return (fd);
}

void
wfor_arp(t_malcolm *m) {
	unsigned char buf[42];
	struct sockaddr_ll from;
	socklen_t lenfrom = sizeof(from);
	ssize_t r = recvfrom(m->fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &lenfrom);
	if (r < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			fprintf(stderr, "[❗]: waiting for arp broadcast..\n");
		else
			f_exit(1, free_malcolm, m, strerror(errno));
		return ;
	}
	t_ether_hdr *eth = (t_ether_hdr *)buf;
	t_arp_hdr *arp = (t_arp_hdr*)(buf + sizeof(t_ether_hdr));
	if (ntohs(arp->op) == ARPOP_REQUEST && ntohs(eth->type) == ETH_P_ARP)
		printf("Got one\n");
}

