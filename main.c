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

void sig_handler(int signum)
{
	(void)signum;

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

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
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
