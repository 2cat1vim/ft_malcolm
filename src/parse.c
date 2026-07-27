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

#define SOURCE 0
#define TARGET 1

#define IPV4_L 4
#define MAC_L 6

static int
settohex(const unsigned char c) {
    if (c >= '0' && c <= '9') 
	    return (c - '0');
    if (c >= 'A' && c <= 'F')
	    return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
	    return (c - 'a' + 10);
    return (-1);
}

static int
mac_pton(const char* src, uint8_t *dst) {
	uint8_t eth[MAC_L];
	zro_mem(&eth, sizeof(eth));
	size_t count = 0;
	while (*src) {
		eth[count] = (settohex(*src) << 4) | settohex(*(src + 1));
		src += 2;
		if (count < ETH_ALEN - 1) {
			if (*src != ':')
				return (-1);
			src++;
		}
		else if (*src != '\0')
			return (-1);
		count++;
	}
	if (count != ETH_ALEN)
		return (-1);

	cpy_mem(dst, eth, MAC_L);
	return (1);
}


static enum P_Answer
resolve_ips(t_malcolm *m, char **av)
{
	struct ifaddrs *ifaddr;

	if (inet_pton(AF_INET, av[SOURCE_IP + 1], &m->src_ip) != 1)
		return (SourceIPBad);
	if (inet_pton(AF_INET, av[TARGET_IP + 1], &m->trg_ip) != 1)
		return (TargetIPBad);
	if (mac_pton(av[SOURCE_MAC + 1], m->src_mac) != 1)
		return (SourceMACBad);
	if (mac_pton(av[TARGET_MAC + 1], m->trg_mac) != 1)
		return (TargetMACBad);

	if (getifaddrs(&ifaddr) == -1) // Create linked list of ifaddrs
		return (NetworkBad);

	for (struct ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;
		if (ifa->ifa_addr->sa_family != AF_INET)
			continue;
		m->itrf = s_dup(ifa->ifa_name);
	}

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
