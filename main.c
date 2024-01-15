/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 02:15:03 by mdor              #+#    #+#             */
/*   Updated: 2024/01/15 02:15:05 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell(t_minishell *data, char **env)
{
	tcgetattr(STDIN_FILENO, &data->original_term);
	data->head = NULL;
	data->first_token = NULL;
	data->node = NULL;
	data->env = copy_env(env);
	data->error_trigger = 0;
	data->code = 0;
}

void	print_nodes(t_minishell *data)
{
	int	i;

	i = 0;
	while (data->node)
	{
		printf("path : %s\n", data->node->path_to_cmd);
		while (data->node->args[i])
		{
			printf("args %d : %s\n", i, data->node->args[i]);
			i++;
		}
		data->node = data->node->next;
	}
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
	char	*prompt;

	prompt = "\033[0;32m 🐚 Minishell > \033[0;37m";
	while (1)
	{
		start_signals(data);
		data->input = readline(prompt);
		if (data->input == NULL)
			ft_exit(data);
		if (data->input != NULL)
			add_history(data->input);
		if (data->input[0] == '\0' || is_only_space(data->input) == 1)
		{
			free(data->input);
			continue ;
		}
		routine(data);
		g_status = 0;
	}
	return ;
}

int	main(int ac, char **av, char **env)
{
	t_minishell	data;

	(void)ac;
	(void)av;
	init_shell(&data, env);
	looping(&data);
	free_tabl(data.env);
	return (data.code);
}
