/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 18:44:16 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/28 15:18:36 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_echo(t_command *cmd)
{
	int i;
	int newline;

	i = 1;
	if (cmd->args[1] && ft_strncmp(cmd->args[1],"-n",3) == 0)
	{
		newline = 0;
		i++;
	}
	
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i],1);
		if (cmd->args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	
}

//chdir ile çalışma dizinini değiştir
void ft_cd(t_command *cmd)
{
	char *path;

	if (!cmd->args[1] || strncmp(cmd->args[1], "~",2) == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
            return;
		}
	}
	else
		path = cmd->args[1];
		
	if (chdir(path) != 0)
	{
		perror("cd");
	}
	
}

void ft_pwd()
{
	char *cwd;

	cwd = getcwd(NULL,0);
	if (cwd)
	{
		printf("%s\n",cwd);
		free(cwd);
	}
	else
		perror("pwd error");
}

//shell değişkenini ortam değişkeni haline getir
//export a env de gözükür 

void ft_export(t_command *cmd)
{
    int i = 1;

    if (!cmd->args[i]) // Eğer argüman yoksa env'yi sıralı göster
    {
        print_env_sorted(cmd->tmp->env);
        return;
    }

    while (cmd->args[i])
    {
        if (!is_valid(cmd->args[i]))
        {
            ft_putstr_fd("minishell: export: `", 2);
            ft_putstr_fd(cmd->args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
        }
        else
        {
            // Sadece '=' varsa ekle
            if (ft_strchr(cmd->args[i], '='))
                update_env(&cmd->tmp->env, cmd->args[i]);
            // '=' yoksa ekleme, sadece hata da yazma
        }
        i++;
    }
}

//envden silmek
void ft_unset(t_command *cmd)
{
	int i = 1;

	while (cmd->args[i])
	{
		if (is_valid(cmd->args[i])) // kontrol koyman iyi olur
			cmd->tmp->env = remove_from_env(cmd->tmp->env, cmd->args[i]);
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		i++;
	}
}

void ft_env(t_command *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd->tmp->env[i] != NULL)
	{
		j = 0;
		while (cmd->tmp->env[i][j] != '\0')
		{
			write(1, &cmd->tmp->env[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int	ft_exit(t_command *cmd)
{
	write(2, "exit\n", 5);

	// Birden fazla argüman varsa, shell kapanmasın ama kod 1 dönsün
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1); // Shell devam eder, exit() çağrılmaz
	}

	// Argüman sayı değilse, shell 255 ile kapanmalı
	if (cmd->args[1] && numeric_control(cmd->args[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit_program(cmd, 255);
	}

	// Geçerli sayısal argüman varsa
	else if (cmd->args[1])
		exit_program(cmd, ft_atoi(cmd->args[1]));
	else
		exit_program(cmd, 0);

	return (0); // Exit çağrıldıktan sonra burası çalışmaz ama syntax için
}
