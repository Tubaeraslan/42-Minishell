/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/07/25 17:06:32 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_two_dimension(char **arg)
{
    int i;

    i = 0;
    if (!arg || !*arg)
        return ;
    while (arg[i])
    {
        if (arg[i])
        {
            free(arg[i]);
            arg[i] = NULL;  
        }
        i++;
    }
    free(arg);
}
void    free_data(t_data *tmp)
{
    if (!tmp)
        return;
    free_two_dimension(tmp->env);
    free(tmp->input);
        tmp->input = NULL;
}


void all_free(t_command *comd, t_data *shell)
{
    if (!comd)
        return;
    //if ((comd->tokens && comd->tokens[0]) || comd->tokens != NULL)
      //  clear_tokens(comd);
    if (comd->cmd && ft_strlen(comd->cmd) > 0)
    {
        free(comd->cmd);
        comd->cmd = NULL;
    }
    if (comd->args && comd->args[0])
    {
        free_two_dimension(comd->args);
        comd->args = NULL;
    }
    clear_command_data(comd);
    free_data(shell);
}

//parsing için lazım
void clear_command_data(t_command *cmd)
{
	if (cmd->infile)
	{
		free(cmd->infile);
		cmd->infile = NULL;
	}
	if (cmd->outfile)
	{
		free(cmd->outfile);
		cmd->outfile = NULL;
	}
	if (cmd->heredoc_limiter)
    {
        free(cmd->heredoc_limiter);
        cmd->heredoc_limiter = NULL;
    }
	cmd->is_pipe = 0;
	cmd->is_append = 0;
    cmd->error_printed = 0;
	cmd->parsing_error = 0;
}
//parsing için
void	free_old_tokens(t_command *command)
{
	int	j;

	if (command->tokens)
	{
		j = 0;
		while (j < command->token_count)
		{
			free(command->tokens[j]);
			command->tokens[j] = NULL;
			j++;
		}
		free(command->tokens);
		command->tokens = NULL;
	}
}

//execute.c için
void free_command_fields(t_command *command)
{
    if (command->cmd)
    {
        free(command->cmd);
        command->cmd = NULL;
    }
    // Eğer varsa args, tokens, redirect vs. de temizlenmeli:
    // free_args(command->args);
    // free_tokens(command->tokens);
    // free_redirects(command->redirects);
    // vs.
}

/*
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
*/