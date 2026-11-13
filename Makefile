NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = coders/main.c \
		coders/utils.c \
		coders/parse.c \
		coders/init.c \
		coders/clean.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) *.swp *.gch a.out

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re