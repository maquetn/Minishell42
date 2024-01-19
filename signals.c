/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:33:49 by mdor              #+#    #+#             */
/*   Updated: 2024/01/19 12:33:27 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int signum)
{
	(void)signum;
	if (g_status == 1)
	{
		ft_putstr_fd("\n", 1);
	}
	else
	{
		if (g_status != 2)
		{
			if (g_status == 0)
				ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void	sigquit(int num)
{
	(void)num;
	if (g_status != 2)
		ft_putstr_fd("Quit: 3\n", 1);
}

void	start_signals(t_minishell *data)
{
	struct termios	terminal;

	terminal = data->original_term;
	terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
	g_status = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
}
