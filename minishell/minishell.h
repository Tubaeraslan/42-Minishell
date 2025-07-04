/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 17:40:25 by ican              #+#    #+#             */
/*   Updated: 2025/06/30 20:03:22 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
typedef struct s_expand
{
	int i;
	int j;
	int single_quote;
	int double_quote;
	char *input;
	char *new_input;
}	t_expand;

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

typedef struct s_tokenizer
{
	int		i;
	int		j;
	int		inside_quotes;
	char	char_quote;
	char	buffer[1024];
	const char *src;
}	t_tokenizer;


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
	char *heredoc_limiter;
	int is_heredoc;
	int heredoc_fd;
	int is_pipe;
	int last_exit_code;
	t_tokenizer tokenizer;
}	t_command;

void parse_input(t_command *command);
int is_valid_syntax(char *input);
int check_pipe(char *input);
int check_redirects(char *input);
void token(t_command *command);
void parsing(t_command *command);
void	execute_commands(t_command *command);
void ft_echo(t_command *cmd);
void ft_pwd();
int ft_exit(t_command *cmd);
void ft_env(t_command *cmd);
void ft_export(t_command *cmd);
void ft_unset(t_command *cmd);
void ft_cd(t_command *cmd);
void add_token(t_command *command, char *buffer);

char	*path_finder(char *cmd, char **env);

int numeric_control(char *arg);
void exit_program(t_command *cmd, int exit_code);
void update_env(char ***envp, char *arg);
int is_valid(char *str);
void print_env_sorted(char **envp);
char **add_to_env(char **env, char *new_var);
char **ft_env_dup(char **envp);
char	**remove_from_env(char **env, const char *var);
void    execute_many_token(t_command	*command);
void	handle_redirections(t_command *cmd);
int	is_built(char *arg);
void execute_built(t_command *cmd);
void setup_heredoc(t_command *cmd);

//free
void free_two_dimension(char **arg);
void    free_data(t_data *tmp);
void    all_free(t_command *cmd);

#endif