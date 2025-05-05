NAME = minishell
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
CC = cc

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c parser.c
OBJ = $(SRC:.c=.o)

all:$(LIBFT) $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBFT)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re