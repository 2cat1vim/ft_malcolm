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
	return ;
}

void
free_malcolm(void *s_) {
	t_malcolm *m;
	m = (t_malcolm *)s_;

	if (m) {
		if (m->itrf)
			free(m->itrf);
		free(m);
	}
}
