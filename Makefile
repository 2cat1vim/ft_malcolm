MAKEFLAGS += -s
NAME = ft_malcolm
CC = clang
CFLAGS = -g -Wall -Wextra -Werror
LIBDIR = lib
LIBNAME = lib.a
SRC = src/main.c \
      src/parse.c \
      src/init.c
OBJS = $(SRC:.c=.o)

all: $(NAME)
	echo "[MAKEFILE]: Compiling ft_malcolm"
	@test -n "$(SILENT_END)" || echo "[MAKEFILE]: Task completed"

$(NAME): $(OBJS) $(LIBDIR)/$(LIBNAME)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBDIR) -l:$(LIBNAME)

$(LIBDIR)/$(LIBNAME):
	$(MAKE) -C $(LIBDIR) -f Makefile

clean:	
	echo "[MAKEFILE]: Cleaning ft_malcolm"
	rm -f $(OBJS)
	$(MAKE) -C $(LIBDIR) -f Makefile clean
	@test -n "$(SILENT_END)" || echo "[MAKEFILE]: Task completed"

fclean:
	echo "[MAKEFILE]: Cleaning ft_malcolm"
	rm -f $(OBJS)
	$(MAKE) -C $(LIBDIR) -f Makefile clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBDIR) -f Makefile fclean
	@test -n "$(SILENT_END)" || echo "[MAKEFILE]: Task completed"

re:
	$(MAKE) fclean SILENT_END=1
	$(MAKE) all

.PHONY: all clean fclean re
