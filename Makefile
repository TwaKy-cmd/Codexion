NAME	= codexion

CC		= cc 

CFLAGS	= -Wall -Werror -Wextra -pthread

SRCDIR	= src

INCDIR	= include

SRCS	=  $(SRCDIR)/main.c 		\
			$(SRCDIR)/parsing.c 	\
			$(SRCDIR)/simulation.c 	\
			$(SRCDIR)/monitor.c		\
			$(SRCDIR)/logs.c		\
			$(SRCDIR)/init.c		\
			$(SRCDIR)/heap.c		\
			$(SRCDIR)/heap_utils.c	\
			$(SRCDIR)/utils.c		\
			$(SRCDIR)/dongle.c		\
			$(SRCDIR)/coder.c		\
			$(SRCDIR)/cleanup.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re