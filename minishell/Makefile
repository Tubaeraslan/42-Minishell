NAME = minishell
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
CC = cc
SRC = main.c parser.c
OBJ = $(SRC:.c=.o)

all:$(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re