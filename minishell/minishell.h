#ifndef MINISHELL_H
# define MINISHELL_H

//git pull --no-rebase

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "libft/libft.h"



typedef struct s_data
{
	char *input;
	char **env;
}	t_data;

typedef struct s_command {
    char **tokens;      // argümanlar (örneğin, ["Hello", "World"])
	int token_count;
	t_data *tmp;
    // Geri yönlendirme, pipe gibi başka alanlar eklenebilir.
} t_command;


void parse_input(t_command *s_command);
void	execute_commands(t_data *command);
void token(t_command *command);

#endif