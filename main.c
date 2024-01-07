#include "minishell.h"
//rajouter un check syntax error entre token et parsing ??

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

char	**copy_env(char **env)
{
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
	data->env = copy_env(env);
}

void	print_nodes(t_minishell *data)
{
	while (data->node)
	{
		int i = 0;
		printf("output : %s\n", data->node->output);
		printf("input : %s\n", data->node->input);
		printf("path : %s\n", data->node->path_to_cmd);
		while (data->node->args[i])
		{
			printf("args %d : %s\n", i, data->node->args[i]);
			i++;
		}
		data->node = data->node->next;
	}
}

// void	free_simple_cmd(t_simple_cmd *cmd)
// {
// 	t_simple_cmd	*temp;
// 	t_simple_cmd	*next;

// 	temp = cmd;
// 	while (temp)
// 	{
// 		free_tabl(temp->args);
// 		free(temp->path_to_cmd);
// 		free(temp->input);
// 		free(temp->output);
// 		temp->prev = NULL;
// 		next = temp->next;
// 		free(temp);
// 		temp = next;
// 	}
// }
/*
int check_parse(char *str)
{
    // faudra rendre cette fct absolument impermeable pour que la suite marche
    int i;

    i = ft_strlen(str);
    if (i > 0 && (str[i - 1] == '>' || str[i - 1] == '<' || str[i - 1] == '.'))
    {
        printf("syntax error near unexpected token `a gerer'\n");
        return (1);
    }
    else
        return (0);
}*/

int	check_if_quotes_are_closed_or_forbidden(char *str)
{
	int	i;
	int	single;
	int	double_quotes;

	i = 0;
	single = 0;
	double_quotes = 0;
	while(str[i])
	{
		if (str[i] == '|' && str[i + 1] == '|')
			return (0);
		else if (str[i] == '"' && double_quotes == 1)
			double_quotes = 0;
		else if (str[i] == '\'' && single == 1)
			single = 0;
		else if (str[i] == ';' || str[i] == '\\')
			return (0);
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
		/*if (check_parse(input))
		{
			free(input);
			continue;
		}*/
		if (check_if_quotes_are_closed_or_forbidden(input) == 0)
		{
			printf("We should not manage that in minishell\n");
			free(input);
			continue;
		}
		token(input, &data);
		if (data.first_token)
			expander(&data);
		//printf("test\n");
		if (data.first_token)
			planting(&data);
		//printf("test\n");
		if (data.node)
			execute_simple_cmd(data.node, &data, NULL);
		//print_nodes(&data);
		free(input);
		//free_simple_cmd(data.node);
		free_custom_alloc(&data);
		status = 0;
	}
	free_tabl(data.env);
	//system("leaks minishell");
	return (0);
}
