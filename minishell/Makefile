NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g
CC = cc
LDFLAGS = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c parse_input.c syntax_check.c lexer.c parsing.c execute.c built-in.c built-in_utils.c all_free_st.c built-in_utils_2.c
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

leaks: 
	@echo "Running Walgring..."
	@valgrind --leak-check=full		\
			--show-leak-kinds=all	\
			--track-origins=yes		\
			--track-fds=yes			\
			--verbose				\
			--suppressions=valgrind.supp	\
			./$(NAME)

.PHONY: all clean fclean re