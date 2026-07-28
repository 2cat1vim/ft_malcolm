/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltrillar <ltrillar@student.42luxembou      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:14:42 by ltrillar          #+#    #+#             */
/*   Updated: 2026/07/28 16:14:44 by ltrillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib.h"

char*
s_dup(const char* str) {
	char* new = NULL;
	char* ptr_new = NULL;
	new = malloc(sizeof(char) * (s_len(str) + 1));
	if (!new)
		return (NULL);
	ptr_new = new;
	while (*str)
		*new++ = *str++;
	*new = '\0';
	return (ptr_new);
}

void*
zro_mem(void* m, size_t n) {
	unsigned char cz = '0';
	size_t i = 0;
	while (i < n)
		((unsigned char*)m)[i++] = cz;
	return (m);
}

void*
cpy_mem(void* dst, const void* src, size_t n) {
	size_t i = 0, j = 0;
	if (!dst && !src && n > 0)
		return (NULL);
	while (i < n)
		((unsigned char*)dst)[j++] = ((unsigned char*)src)[i++];
	return (dst);
}

int
cmp_mem(const void* s1, const void* s2, size_t n) {
	size_t i = 0, j = 0;
	while (i < n) {
		if (((unsigned char *)s1)[j++] != ((unsigned char*)s2)[i++])
			return (((unsigned char *)s1) - ((unsigned char*)s2));
	}
	return (0);
}		
