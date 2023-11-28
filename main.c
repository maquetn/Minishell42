/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 13:39:04 by nmaquet           #+#    #+#             */
/*   Updated: 2023/11/27 20:17:51 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int status = 0;

void	restore_terminal(struct termios *original_termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, original_termios) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

// Signal handler function for SIGINT (Ctrl+C)
void sig_handler(int signum)
{
	(void)signum; // To avoid the unused parameter warning

	struct termios	original_termios;

	tcgetattr(STDIN_FILENO, &original_termios);
	original_termios.c_lflag &= ~ ECHOCTL;
	rl_replace_line ("", 0);
	rl_on_new_line();
	rl_redisplay ();
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
	printf("\n\033[0;32m 🐚 Minishell > \033[0;37m");
	status = 1;
}

int main(void)
{
	char *prompt;
	char *input;

	prompt = "\033[0;32m 🐚 Minishell > \033[0;37m";

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN); // back slash

	while (1)
	{
		if (status != 1)
			input = readline(prompt);
		if (input != NULL)
			token(input);
		if (input == NULL || strcmp(input, "exit") == 0)
		{
			printf("\n       \e[0;35m\e[46m                                                             \033[0;37m");
			printf("\n       \e[0;35m\e[46m   +++++++++++++++++++++++++++++++++++++++++++++++++++++++   \033[0;37m");
			printf("\n       \e[0;35m\e[46m   +                                                     +   \033[0;37m");
			printf("\n       \e[0;35m\e[46m   +      🐚 🐚 🐚    Exiting Minishell.    🐚 🐚 🐚     +   \033[0;37m");
			printf("\n       \e[0;35m\e[46m   +                                                     +   \033[0;37m");
			printf("\n       \e[0;35m\e[46m   +++++++++++++++++++++++++++++++++++++++++++++++++++++++   \033[0;37m");
			printf("\n       \e[0;35m\e[46m                                                             \033[0;37m\n");
			break;
		}
		if (input != 0)
			add_history(input);
		free(input);
		status = 0;
	}

	return 0;
}
