/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_st.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ican <<ican@student.42.fr>>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:09:21 by ican              #+#    #+#             */
/*   Updated: 2025/07/05 11:23:14 by ican             ###   ########.fr       */
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
}


void    all_free(t_command *cmd)
{
    if (!cmd)
        return;
    free_two_dimension(cmd->tokens);
    free(cmd->cmd);
    free_two_dimension(cmd->args);
    free(cmd->infile);
    free(cmd->outfile);
    free_data(cmd->tmp);
    free(cmd->next);
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