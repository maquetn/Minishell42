/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 16:33:49 by mdor              #+#    #+#             */
/*   Updated: 2024/01/14 16:33:50 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void sig_handler(int signum)
{
	//cat + ctrl+C
    (void)signum; // To avoid the unused parameter warning
	if (g_status)
	{
		ft_putstr_fd("\n", 1);
		g_status = 0;
	}
	else
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigquit(int num)
{
	(void)num;
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
