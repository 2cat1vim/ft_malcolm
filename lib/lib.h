/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltrillar <ltrillar@student.42luxembou      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 16:14:37 by ltrillar          #+#    #+#             */
/*   Updated: 2026/07/28 16:14:38 by ltrillar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_H
#define LIB_H

#include <unistd.h>
#include <stdlib.h>

/* START EXIT.C */
//exit with error|success while passing a function with a parameter whiiiile also printing a message.
void f_exit(int code, void (*f)(void*), void* s_, const char* str);
// exit with error while passing a function with a parameter
void ffe_exit(void (*f)(void*), void* s_);
// exit with success while passing a function with a parameter
void ffs_exit(void (*f)(void*), void* s_);
// exit with error while printing a message
void ep_exit(const char* str);
// exit with success while printing a message
void sp_exit(const char* str);
// exit with error
void e_exit(void);
// exit with success
void s_exit(void);
/* END EXIT.C */

/* START LEN.C */
// calculate len of a str, if str == NULL > return 0, else return len
size_t s_len(const char *s);
/* END LEN.C */

/* START MEMORY.C */
char* s_dup(const char* str);
void* zro_mem(void* m, size_t n);
void* cpy_mem(void* dst, const void* src, size_t n);
int cmp_mem(const void* s1, const void* s2, size_t n);
/* END MEMORY.C */
#endif
