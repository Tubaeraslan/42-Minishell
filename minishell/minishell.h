#ifndef MINISHELL_H
# define MINISHELL_H

//git pull --no-rebase

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

typedef struct s_data
{
	char *input;
	char **env;
}	t_data;

typedef struct s_command {
    char *cmd;          // komut (örneğin, "echo")
    char **args;        // argümanlar (örneğin, ["Hello", "World"])
	t_data *tmp;
    // Geri yönlendirme, pipe gibi başka alanlar eklenebilir.
} t_command;


void parse_input(t_data *shell);

#endif