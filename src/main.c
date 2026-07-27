#include "../include/malcolm.h"

static volatile int Running = 1;

void
handle_sigint(int sig) {
	(void)sig;
	printf("\n");
	Running = 0;
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
main(int ac, char** av)
{
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

	signal(SIGINT, handle_sigint);

	t_malcolm *m = malloc((sizeof(t_malcolm) * 1));
	if (!m)
		ep_exit("t_malcolm allocation failed");

	init_malcolm(m);
	if (!parse_point(m, av))
		ffe_exit(free_malcolm, m);

	malcolm_header();

	if (m->itrf)
		printf("[🚩]: found interface: %s\n", m->itrf);
	
	m->fd = crt_sock(m);
	if (m->fd == -1)
		f_exit(1, free_malcolm, m, "failed to create socket");

	while (Running) {
		wfor_arp(m);
	}
	ffe_exit(free_malcolm, m);
}
