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
	//cat + ctrl+C
    (void)signum; // To avoid the unused parameter warning

    struct termios original_termios;

    tcgetattr(STDIN_FILENO, &original_termios);
    original_termios.c_lflag &= ~ECHOCTL;

    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();

    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);

    printf("\n\033[0;32m 🐚 Minishell > \033[0;37m");
    status = 1;
}

char	**copy_env(char **env, t_minishell *data)
{
	(void)data;
	int count;
	char	**copy;
	int	i;

	count = 0;
	i = 0;
	while (env[count] != NULL)
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	while (i < count)
	{
		copy[i] = strdup(env[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

void	init_shell(t_minishell *data, char **env)
{
	data->head = NULL;
	data->first_token = NULL;
	data->node = NULL;
	data->env = copy_env(env, data);
	data->error_trigger = 0;
}

void	print_nodes(t_minishell *data)
{
	while (data->node)
	{
		int i = 0;
		//printf("output : %s\n", data->node->output);
		//printf("\ninput : %s\n", data->node->input);
		printf("path : %s\n", data->node->path_to_cmd);
		while (data->node->args[i])
		{
			printf("args %d : %s\n", i, data->node->args[i]);
			i++;
		}
		data->node = data->node->next;
	}
}

int	check_if_quotes_are_closed(char *str)
{
	int	i;
	int	single;
	int	double_quotes;

	i = 0;
	single = 0;
	double_quotes = 0;
	while(str[i])
	{
		if (str[i] == '"' && double_quotes == 1)
			double_quotes = 0;
		else if (str[i] == '\'' && single == 1)
			single = 0;
		else if (str[i] == '"')
			double_quotes = 1;
		else if (str[i] == '\'')
			single = 1;
		i++;
	}
	if (single != 0 || double_quotes != 0)
		return (0);
	return (1);
}

int	is_only_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
	{
		if (str[i] != ' ' && str[i] != '\0')
			return (0);
		i++;
	}
	if (i >= ft_strlen(str))
		return (1);
	return (0);
}

void	looping(t_minishell *data)
{
	char *prompt;
	char *input;

	prompt = "\033[0;32m 🐚 Minishell > \033[0;37m";
	while (1)
	{
		if (status == 1)
			break;
		if (status != 1)
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
		if (input[0] == '\0' || is_only_space(input) == 1)
		{
			free(input);
			continue;
		}
		if (input != NULL)
       		add_history(input);
		// if (check_if_quotes_are_closed_or_forbidden(input) == 0)
		// {
		// 	printf("We should not manage that in minishell\n");
		// 	free(input);
		// 	continue;
		// }
		token(input, data);
		if (data->first_token && data->error_trigger == 0)
			expander(data);
		if (data->first_token && data->error_trigger == 0)
			planting(data);
		if (data->node && data->error_trigger == 0)
			execute_simple_cmd(data->node, data, NULL);
		//print_nodes(&data);
		free(input);
		free_custom_alloc(data);
		data->exit_code = data->error_trigger;//je prep la logique de exit code ici
		data->error_trigger = 0;
		status = 0;
	}
	return ;
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_minishell	data;

	init_shell(&data, env);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN); // back slash
	looping(&data);
	free_tabl(data.env);
	//system("leaks minishell");
	return (data.exit_code);
}
