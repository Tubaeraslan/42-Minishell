#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "libft/libft.h"
#include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct s_data
{
	char *input;
	char **env;
}	t_data;

typedef enum e_redirect_type
{
	REDIR_IN,   //<
	REDIR_OUT,	//>
	REDIR_APPEND,	//>>
	REDIR_HEREDOC	//<<
}	t_redirect_type;

typedef struct s_redirects
{
	t_redirect_type type;
	char *name; //yönlendirme yapılan dosya adı
	struct s_redirects *next;
}	t_redirects;

typedef struct s_command
{
	char **tokens; //["ls" , "-l", "cat", "hello","|"]
	int token_count;
	char *cmd;  //komut listesi
	char **args; //argüman listesi
	char *infile; // <dosya kullanıdlığında
	char *outfile; // > >> dosya kullanıldığında
	int is_append; // > yada >> mı kontrol
	t_redirects *redirects;
	struct s_command	*next;  //pipelara bölünce bir sonraki komutu tutmak için
	t_data *tmp;
	int is_pipe;
	int fd[2]; // 
	pid_t pid;
}	t_command;

void parse_input(t_command *command);
int is_valid_syntax(char *input);
int check_pipe(char *input);
int check_redirects(char *input);
void token(t_command *command);
void parsing(t_command *command);

#endif