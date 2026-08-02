#include "lib.h"

size_t s_len(const char *s) {
	size_t cnt = 0;
	if (!s)
		return (cnt);
	while (*s)
		s++, cnt++;
	return (cnt);
}
