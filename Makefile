NAME = program.exe
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = coders/main.c \
		coders/utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) *.swp *.gch *.exe

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re