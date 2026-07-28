/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltrillar <ltrillar@student.42luxembou      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:13:55 by ltrillar          #+#    #+#             */
/*   Updated: 2026/07/28 16:13:56 by ltrillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/malcolm.h"

#define IPV4_L 4
#define MAC_L 6

void
init_malcolm(t_malcolm *m) {
	zro_mem(m->src_ip, IPV4_L);
	zro_mem(m->trg_ip, IPV4_L);
	zro_mem(m->src_mac, MAC_L);
	zro_mem(m->trg_mac, MAC_L);
	m->itrf = NULL;
	m->fd = -1;
	return ;
}

void
free_malcolm(void *s_) {
	t_malcolm *m;
	m = (t_malcolm *)s_;

	if (m) {
		if (m->itrf)
			free(m->itrf);
		if (m->fd != -1)
			close(m->fd);
		free(m);
	}
}
