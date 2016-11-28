CC = gcc

CFLAGS = -Wall -Wextra -g2

SRCS =	main.c \
	get_next_line.c \
	disp_tools.c

OBJS = $(SRCS:.c=.o)

NAME = gnl

all:		$(NAME)

$(NAME):	$(OBJS)
		$(CC) -o $(NAME) $(OBJS)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY: 	all clean fclean re
