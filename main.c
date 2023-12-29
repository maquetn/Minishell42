#include "minishell.h"

int g_exit_code = 0;

void	restore_terminal(struct termios *original_termios)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, original_termios) == -1)
	{
		perror("tcsetattr");
		ft_exit(EXIT_FAILURE, NULL);
	}
}

void sig_handler(int signum)
{
	(void)signum;

	struct termios original_termios;

	tcgetattr(STDIN_FILENO, &original_termios);
	original_termios.c_lflag &= ~ECHOCTL;

	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();

	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);

	printf("\n\033[0;32m 🐚 Minishell > \033[0;37m");
	g_exit_code = 1;
}

char	**copy_env(char **env)
{
	int count;
	char	**copy;
	int	i;

	count = 0;
	i = 0;
	while (env[count] != NULL)
		count++;
	copy = (char **)malloc(sizeof(char *) * (count + 1));
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

void	init_shell(t_minishell *data, char **env)
{
	data->first_token = NULL;
	data->node = NULL;
	data->env = copy_env(env);
}

void	print_nodes(t_minishell *data)
{
	while (data->node)
	{
		int i = 0;

		while (data->node->args[i])
			i++;
		data->node = data->node->next;
	}
}

void	free_simple_cmd(t_simple_cmd *cmd)
{
	t_simple_cmd	*temp;
	t_simple_cmd	*next;

	temp = cmd;
	while (temp)
	{
		free_tabl(temp->args);
		free(temp->path_to_cmd);
		free(temp->input);
		free(temp->output);
		temp->prev = NULL;
		next = temp->next;
		free(temp);
		temp = next;
	}
}

void expand_tokens(t_token **head, char **env)
{
    t_token *current = *head;
    t_token *prev = NULL;

    while (current != NULL)
	{
        if (current->type == STR && strchr(current->content, '$') != NULL)
		{
            char *expanded_content = expand_env_variables(current->content, env);
            current->content = expanded_content;
        }

        prev = current;
        current = current->next;
    }
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *prompt;
	char *input;
	t_minishell	data;

	init_shell(&data, env);
	prompt = "\033[0;32m 🐚 Minishell > \033[0;37m";

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN); // back slash

	while (1)
	{
		if (g_exit_code == 1)
			break;
		if (g_exit_code != 1)
			input = readline(prompt);
		if (input == NULL || strcmp(input, "exit") == 0)
		{
			printf("\n");
			printf("       \e[1;33m\e[44m ********************************************************* \033[0;37m\n");
			printf("       \e[1;33m\e[44m *                                                       * \033[0;37m\n");
			printf("       \e[1;33m\e[44m *        🐚 🐚 🐚    Exiting Minishell.    🐚 🐚 🐚     * \033[0;37m\n");
			printf("       \e[1;33m\e[44m *                                                       * \033[0;37m\n");
			printf("       \e[1;33m\e[44m ********************************************************* \033[0;37m\n\n");
			free(input);
			break;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}

		add_history(input);
		token(input, &data);
		if (data.first_token)
		{
			expand_tokens(&data.first_token, data.env);
			planting(&data);
		}
		if (data.node)
			execute_simple_cmd(data.node, &data);
		//print_nodes(&data);
		free(input);
		free_simple_cmd(data.node);
		
	}
	free_tabl(data.env);
	//system("leaks minishell");
	return 0;
}
