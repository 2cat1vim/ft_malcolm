#include "../include/malcolm.h"

void
init_malcolm(t_malcolm *m) {
	for (size_t i = 0; i < 2; i++)
		zro_mem(&m->ips[i], sizeof(struct in_addr));
	for (size_t i = 0; i < 2; i++)
		zro_mem(&m->macs[i], sizeof(struct ether_addr));
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
