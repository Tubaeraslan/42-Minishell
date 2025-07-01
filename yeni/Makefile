NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g
CC = cc
LDFLAGS = -lreadline

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c signal.c env_function.c parse_input.c pipe_syntax.c syntax_check.c expand.c \
		lexer.c parsing.c parsing_utils.c execute.c execute_utils.c execute_no_pipe.c \
		built-in.c cd.c echo.c env.c exit.c export.c pwd.c unset.c env_utils.c \
		execute_with_pipe.c
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