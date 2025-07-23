/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:04:57 by teraslan          #+#    #+#             */
/*   Updated: 2025/07/23 19:09:47 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <sys/types.h>
# include <sys/wait.h> 
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>

extern int g_signal_status;

typedef struct s_data
{
	char	*input;
	char	**env;
}	t_data;

typedef struct s_expand
{
	int		i;
	int		j;
	int		single_quote;
	int		double_quote;
	char	*input;
	char	*new_input;
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

typedef struct s_GarbageCollector
{

}	s_GarbageCollector;

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
	int parsing_error;
	int error_printed;
	t_tokenizer tokenizer;
	s_GarbageCollector collector;
}	t_command;


void		handle_signals();
char		**ft_env_dup(char **envp);
void		parse_input(t_command *command);
int			is_valid_syntax(char *input);
int			check_pipe(char *input);
int			check_redirects(char *input);
void		expand_variables(t_command *command);
char		*get_env_value(char **env, char *key);
void		clear_tokens(t_command *command);
int			process_quotes(t_tokenizer *tk);
void		process_space(t_command *command, t_tokenizer *tk);
int			process_operator(t_command *command, t_tokenizer *tk);
void		add_token(t_command *command, char *buffer);
void		parsing(t_command *command);
int			handle_infile(t_command *command, char **tokens, int *i);
int			handle_outfile(t_command *command, char **tokens, int *i, int append);
int			handle_heredoc(t_command *command, char **tokens, int *i);
t_command	*create_next_command(t_command *command, char **tokens, int index);
void		update_args_and_cmd(t_command *command, char **args, int count);
void		execute_commands(t_command *command);
void		setup_heredoc(t_command *cmd);
int			is_built(char *arg);
pid_t		handle_fork(t_command *cmd, int prev_fd, int *fd);
void		execute_builtin_with_redir(t_command *command);
void		handle_fork_error(void);
int			handle_redirections(t_command *cmd);
void		execute_built(t_command *cmd);
void		execute_a_token(t_command *command);
void		execute_child_process(t_command *command);
void		ft_echo(t_command *cmd);
void		ft_pwd();
void		ft_env(t_command *cmd);
int			ft_exit(t_command *cmd);
void		ft_export(t_command *cmd);
void		ft_unset(t_command *cmd);
void		ft_cd(t_command *cmd);
int			is_valid(char *str);
char		**sort_env(char **env);
void		free_env(char **env);
void		update_env(char ***envp, char *arg);
int			execute_many_token(t_command *command);
char		*path_finder(char *cmd, char **env);
void		sigint_handler(int sig);
int			get_oldpwd(t_command *cmd, char *cwd, size_t c_size, char **oldpwd);
int			get_path(t_command *cmd, char **path);
void		exec_external_or_exit(t_command *cmd);
int			execute_pipeline(t_command *cmd, pid_t *pids);
int			wait_for_children(pid_t *pids, int count, pid_t last_pid);
void		parse_error(t_command *command, const char *msg);
int			infile_error(t_command *cmd, char *file, const char *msg, int ex_c);
int			outfile_error(t_command *command, char *file, const char *msg, int ex_c);
void		copy_tokens(t_command *next_cmd, char **tokens, int start, int count);

void		free_two_dimension(char **arg);
void		all_free(t_command *comd, t_data *shell);
void		clear_command_data(t_command *cmd);
void		free_data(t_data *tmp);

#endif