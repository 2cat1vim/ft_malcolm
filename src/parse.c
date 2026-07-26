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
mac_pton(const char* src, struct ether_addr *dst) {
	uint8_t eth[ETH_ALEN];
	memset(&eth, 0, sizeof(eth));
	size_t count = 0;
	while (*src) {
		eth[count] = (settohex(*src) << 4) | settohex(*(src + 1));
		src += 2;
		if (count < ETH_ALEN - 1) {
			if (*src != ':')
				return (-1);
			src++;
		}
		count++;
	}
	if (count != ETH_ALEN)
		return (-1);

	memcpy(dst, eth, sizeof(eth));
	return (1);
}


static enum P_Answer
resolve_ips(t_malcolm *m, char **av)
{
	struct ifaddrs *ifaddr;
	bool match[4] = {false, false, false, false};

	if (inet_pton(AF_INET, av[SOURCE_IP + 1], &m->ips[SOURCE]) != 1)
		return (SourceIPBad);
	if (inet_pton(AF_INET, av[TARGET_IP + 1], &m->ips[TARGET]) != 1)
		return (TargetIPBad);
	if (mac_pton(av[SOURCE_MAC + 1], &m->macs[SOURCE]) != 1)
		return (SourceMACBad);
	if (mac_pton(av[TARGET_MAC + 1], &m->macs[TARGET]) != 1)
		return (TargetMACBad);

	if (getifaddrs(&ifaddr) == -1) // Create linked list of ifaddrs
		return (NetworkBad);

	for (struct ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr)
			continue;
	
        	if (ifa->ifa_addr->sa_family == AF_PACKET) {
            		struct sockaddr_ll *sll = (struct sockaddr_ll *)ifa->ifa_addr;
            		if (memcmp(sll->sll_addr, &m->macs[SOURCE], ETH_ALEN) == 0)	
                		match[SOURCE_MAC] = true;
            		if (memcmp(sll->sll_addr, &m->macs[TARGET], ETH_ALEN) == 0)
                		match[TARGET_MAC] = true;
		}

		if (ifa->ifa_addr->sa_family == AF_INET) {

			struct in_addr addr = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			if (addr.s_addr == m->ips[SOURCE].s_addr) {
				m->itrf = strdup(ifa->ifa_name);
				match[SOURCE_IP] = true;
			}
			if (addr.s_addr == m->ips[TARGET].s_addr)
				match[TARGET_IP] = true;
		}
	}

	for (size_t error_code = 0; error_code < 4; error_code++) {
		if (!match[error_code])
			return ((enum P_Answer)error_code);
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
