#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <stdlib.h>

typedef struct s_data
{
	char *input;
	char **env;
}	t_data;

#endif