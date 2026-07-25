#include "../include/malcolm.h"

enum P_Answer {
	SourceIPBad,
	SourceMACBad,
	TargetIPBad,
	TargetMACBad,
	NetworkBad,
	Good
};

#define SOURCE_IP 0
#define SOURCE_MAC 1
#define TARGET_IP 2
#define TARGET_MAC 3

/*
static mac_pton() {
}
*/

static enum P_Answer
resolve_ips(t_malcolm *m, char **av)
{
	(void)m;
	char *interface = NULL;
	struct ifaddrs *ifaddr;
	struct in_addr ips[2];
	struct ether_addr macs[2];
	bool match[4] = {false, false, false, false};

	if (inet_pton(AF_INET, av[SOURCE_IP + 1], &ips[SOURCE_IP]) != 1)
		return (SourceIPBad);
	if (inet_pton(AF_INET, av[TARGET_IP + 1], &ips[TARGET_IP]) != 1)
		return (TargetIPBad);

	if (getifaddrs(&ifaddr) == -1) // Create linked list of ifaddrs
		return (NetworkBad);

	for (struct ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;
	
        	if (ifa->ifa_addr->sa_family == AF_PACKET) {
            		struct sockaddr_ll *sll = (struct sockaddr_ll *)ifa->ifa_addr;
            		if (sll->sll_halen == 6 && memcmp(sll->sll_addr, &mac[0], 6) == 0)
                		match[SOURCE_MAC] = true;
            		if (sll->sll_halen == 6 && memcmp(sll->sll_addr, &mac[1], 6) == 0)
                		match[TARGET_MAC] = true;

		if (ifa->ifa_addr->sa_family == AF_INET) {

			struct in_addr addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			if (addr.s_addr == ips[SOURCE_IP].s_addr) {
				interface = ifa->ifa_name;
				match[SOURCE_IP] = true;
			}
			if (addr.s_addr == ips[TARGET_IP].s_addr)
				match[TARGET_IP] = true;
		}
	}

	for (size_t error_code = 0; error_code < IP_SIZE; error_code++) {
		if (!match[error_code])
			return ((enum P_Answer)error_code);
	}

	if (interface)
		printf("Found available interface: %s\n", interface);

	freeifaddrs(ifaddr);
	return (Good);
}

bool
parse_point(t_malcolm *m, char **av) {
	enum P_Answer a = resolve_ips(m, av);
	switch (a) {
		case SourceIPBad:
			fprintf(stderr, "ft_malcolm: source ip is bad\n");
			break;
		case TargetIPBad:
			fprintf(stderr, "ft_malcolm: target ip is bad\n");
			break;
		case SourceMACBad:
			fprintf(stderr, "ft_malcolm: source mac is bad\n");
			break;
		case TargetMACBad:
			fprintf(stderr, "ft_malcolm: target mac is bad\n");
			break;
		case NetworkBad:
			fprintf(stderr, "ft_malcolm: cannot recreate local network\n");
			break;
		case Good:
			return (true);
	}
	return (false);
}
