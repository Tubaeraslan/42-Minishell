/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_many.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:52:32 by ican              #+#    #+#             */
/*   Updated: 2025/06/29 16:56:30 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute_many_token(t_command	*command)
{
	int fd[2];
	int pre_fd;
	pid_t pid;
	char *path;

	pre_fd = -1; //önceki pipe ın okuma ucu -1 demek daha önce pipe yapmadık ilk pipe dayız stdin terminalden değer alınıcak

	while (command)
	{
		if (command->next) //sonraki komut varsa pipe aç
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			
		}
		if (command->is_heredoc)
				setup_heredoc(command);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) //child process
		{
			//ilk pipe da değiliz cmd1| cmd2 olursa eğer cmd1 in stdin gibi davranması lazım-önceki komutun çıktısının terminalden gelen bir değer gibi çalışması lazım
			if (pre_fd != -1)
			{
				dup2(pre_fd,STDIN_FILENO);  //stdin pipe okuma ucu
				close(pre_fd);
			}
			//pipe dan gelen değeri bir sonraki pipe a aktarmamız lazım o yüzden okuma ucu kapanır yazma ucu da stdout ile gelen değeri tutar
			if (command->next)
			{
				close(fd[0]); //okuma ucu kapandı
				dup2(fd[1],STDOUT_FILENO); //stdout yazma ucu
				close(fd[1]); //yazma ucu kapandı
			}
			handle_redirections(command);
			if (is_built(command->cmd))
			{
				execute_built(command);
				exit(0);
			}
			else
			{
				path = path_finder(command->cmd,command->tmp->env);
				if (!path)
				{
					ft_putstr_fd("command not found: ", 2);
					ft_putstr_fd(command->cmd, 2);
					ft_putchar_fd('\n', 2);
					exit(127);
				}
				execve(path, command->args, command->tmp->env);
				perror("execve");
				exit(EXIT_FAILURE);
			}
			
		}
		else //parent  //chid da execve yapıldı burda kontrol ve pipe yönetimi yapılcak
		{
			//okuma ucu kapatıldı
			if (pre_fd != -1)
				close(pre_fd);
			//bir daha pipe varsa yazma ucu kapansın okuma ucu bir önceki pipe ın çıktısı olsun
			if (command->next)
			{
				close(fd[1]);
				pre_fd = fd[0];
			}
			else
				close(fd[0]);  //son komutsa okuma ucunu kapat
			waitpid(pid, NULL, 0);
			command = command->next;
		}
		
	}
	
	

}
