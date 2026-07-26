#include "../include/malcolm.h"

void
init_malcolm(t_malcolm *m) {
	for (size_t i = 0; i < 2; i++)
		memset(&m->ips[i], 0, sizeof(struct in_addr));
	for (size_t i = 0; i < 2; i++)
		memset(&m->macs[i], 0, sizeof(struct ether_addr));
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
