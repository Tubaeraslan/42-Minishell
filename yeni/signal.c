/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: teraslan <teraslan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:07:21 by teraslan          #+#    #+#             */
/*   Updated: 2025/06/30 18:08:11 by teraslan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void sigint_handler(int sig)
{
	(void)sig;
	write(2,"\n",1);
	rl_replace_line("",0);  //mevcut satırı temizle
	rl_on_new_line();  //readline a yeni satır olduğunu bildir
	rl_redisplay();   //promptu ve satırı yeniden yaz
}

void handle_signals()
{
	//ctrl-c
	signal(SIGINT, sigint_handler);
	//ctrl-\'
	signal(SIGQUIT,SIG_IGN);
}