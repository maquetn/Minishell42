#include "minishell.h"

void	init_shell(t_minishell *data, char **env)
{
	tcgetattr(STDIN_FILENO, &data->original_term);
	data->head = NULL;
	data->first_token = NULL;
	data->node = NULL;
	data->env = copy_env(env);
	data->error_trigger = 0;
	data->exit_code = 0;
}

void	print_nodes(t_minishell *data)
{
	while (data->node)
	{
		int i = 0;
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

	prompt = "\033[0;32m 🐚 Minishell > \033[0;37m";
	while (1)
	{
		start_signals(data);
		data->input = readline(prompt);
		if (data->input == NULL)
			ft_exit(data);
		if (data->input!= NULL)
       		add_history(data->input);
		if (data->input[0] == '\0' || is_only_space(data->input) == 1)
		{
			free(data->input);
			continue;
		}
		token(data->input, data);
		if (data->first_token && data->error_trigger == 0)
			expander(data);
		if (data->first_token && data->error_trigger == 0)
			planting(data);
		signal(SIGQUIT, sigquit);
		if (data->node && data->error_trigger == 0)
			execute_simple_cmd(data->node, data, NULL);
		//print_nodes(&data);
		free(data->input);
		free_custom_alloc(data);
		data->exit_code = data->error_trigger;
		data->error_trigger = 0;
		g_status = 0;
	}
	return ;
}

int main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	t_minishell	data;

	init_shell(&data, env);
	looping(&data);
	free_tabl(data.env);
	//system("leaks minishell");
	return (data.exit_code);
}
