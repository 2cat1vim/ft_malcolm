#include "lib.h"

#define SUCCESS 0
#define ERROR 1
#define PRINT_ERROR 2

// EXIT WITH SUCCESS/ERROR FREEING A STRUCT AND PRITING A MESSAGE.
// EACH CASE ARE MODIFIABLE BY SUB FUNCTION DEFINED HERE.

void
f_exit(int code, void (*f)(void*), void* s_, const char* str) {
	if (f && s_)
		f(s_);
	if (str && code == ERROR) {
		write(PRINT_ERROR, "error: ", s_len("error: "));
		write(PRINT_ERROR, str, s_len(str));
		write(PRINT_ERROR, "\n", 1);
	}
	if (code == ERROR)
		exit(ERROR);
	exit(SUCCESS);
}

void
ffe_exit(void (*f)(void*), void* s_) {
	f_exit(ERROR, f, s_, NULL);
}

void
ffs_exit(void (*f)(void*), void* s_) {
	f_exit(SUCCESS, f, s_, NULL);
}

void
ep_exit(const char* str) {
	f_exit(ERROR, NULL, NULL, str);
}

void
sp_exit(const char *str) {
	f_exit(SUCCESS, NULL, NULL, str);
}

void
e_exit(void) {
	f_exit(ERROR, NULL, NULL, NULL);
}

void
s_exit(void) {
	f_exit(SUCCESS, NULL, NULL, NULL);
}
