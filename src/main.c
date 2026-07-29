/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltrillar <ltrillar@student.42luxembou      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:13:28 by ltrillar          #+#    #+#             */
/*   Updated: 2026/07/29 12:58:53 by ltrillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/malcolm.h"

static volatile bool stop = false;

void
handle_sigint(int sig) {
	(void)sig;
	printf("\n");
	stop = true;
}

static void
malcolm_header(void) {
    printf("\033[35m███▄ ▄███▓ ██▓     ▄████▄   ███▄ ▄███▓\n");
    printf("▓██▒▀█▀ ██▒▓██▒    ▒██▀ ▀█  ▓██▒▀█▀ ██▒\n");
    printf("▓██    ▓██░▒██░    ▒▓█    ▄ ▓██    ▓██░\n");
    printf("▒██    ▒██ ▒██░    ▒▓▓▄ ▄██▒▒██    ▒██ \n");
    printf("▒██▒   ░██▒░██████▒▒ ▓███▀ ░▒██▒   ░██▒\n");
    printf("░ ▒░   ░  ░░ ▒░▓  ░░ ░▒ ▒  ░░ ▒░   ░  ░\n");
    printf("░  ░      ░░ ░ ▒  ░  ░  ▒   ░  ░      ░\n");
    printf("░      ░     ░ ░   ░        ░      ░   \n");
    printf("       ░       ░  ░░ ░             ░   \n");
    printf("                   ░                   \033[0m\n");
}

int
main(int ac, char** av) {
	(void)av;
	if (getuid() != 0)
		ep_exit("Require uid to 0 (root)");
	if (ac != 5) {
		ep_exit("format:\nsudo ./ft_malcolm\n"
			       	"	[source-ip]\n"
			       	"	[source-mac-addr]\n"
			       	"	[target-ip]\n"
			       	"	[target-mac-addr]\n\n[!]: it must be in this exact order");
	}

	struct sigaction s;
	s.sa_handler = handle_sigint;
	zro_mem(&s.sa_mask, sizeof(s.sa_mask));
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);

	t_malcolm *m = malloc((sizeof(t_malcolm) * 1));
	if (!m)
		ep_exit("t_malcolm allocation failed");

	init_malcolm(m);
	if (!parse_point(m, av))
		ffe_exit(free_malcolm, m);

	malcolm_header();

	if (!m->itrf) 
		f_exit(BAD, free_malcolm, m, "[❗]: no interface found");
	printf("found interface: %s\n", m->itrf);
	
	m->fd = crt_sock(m);
	if (m->fd == -1)
		f_exit(BAD, free_malcolm, m, "failed to create socket");

	while (!stop) {
		t_arp_hdr *arp = NULL;
		int r = wfor_arp(m, &arp);
		if (r == -1)
			f_exit(BAD, free_malcolm, m, strerror(errno));
		if (r == 1)
			continue;
		if (sto_arp(m, arp) == -1)
			f_exit(BAD, free_malcolm, m, strerror(errno));
		break;
	}
	f_exit(GOOD, free_malcolm, m, "Sent an ARP reply packet, you may now check the arp table on the target.\nExiting program...");	
	ffe_exit(free_malcolm, m);
}
