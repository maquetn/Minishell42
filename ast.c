/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:39:04 by mdor              #+#    #+#             */
/*   Updated: 2023/12/08 09:39:06 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	*get_env(char *name, char **env)
{
	char	*path;
	int		i;
	int		j;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		path = ft_substr(env[i], 0, j);
		if (strcmp(path, name) == 0)
		{
			free(path);
			return (env[i] + j + 1);
		}
		free(path);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	*executable;
	int		i;
	char	**paths;
	char	*potential_path;

	i = -1;
	paths = ft_split(get_env("PATH", env), ':');
	if (!paths)
		EXIT_FAILURE;
	while (paths[++i])
	{
		potential_path = ft_strjoin(paths[i], "/");
		executable = ft_strjoin(potential_path, cmd);
		free(potential_path);
		if (access(executable, F_OK | X_OK) == 0)
			{
				free_tabl(paths);
				return(executable);
			}
		free(executable);
	}
	free_tabl(paths);
	return (strdup(cmd));
}

void	free_tokens(t_token *token)
{
	t_token *temp;
	t_token	*next_token;

	temp = token;
	while (temp)
	{
		//printf("token content %s\n", temp->content);
		free(temp->content);
		//printf("token content %s\n", token->content);
		next_token = temp->next;
		temp->prev = NULL;
		free(temp);
		temp = next_token;
		//printf("freed one token\n");
	}
}

void	init_simple_cmd(t_simple_cmd *cmd)
{
	cmd->path_to_cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->prev = NULL;
    cmd->next = NULL;
}

t_simple_cmd	*get_cmd(t_token *token, t_minishell *data)
{
	t_simple_cmd	*cmd;
	
	cmd = create_simple_cmd(data, token);
	return (cmd);
}

t_simple_cmd	*recursive_parsing(t_minishell *data)
{
	t_simple_cmd	*prev;
	t_simple_cmd	*next;

	//printf("first token value : %s\n", data->first_token->content);
	prev = get_cmd(data->first_token, data);
	//printf("after first prev\n");
	if (data->first_token)
	{
		//printf("first token value : %s\n", data->first_token->content);
		//printf("entered \n");
		next = recursive_parsing(data);
		prev->next = next;
		next->prev = prev;
	}
	return (prev);
}

void	planting(t_minishell *data)
{
	t_token	*first_token;

	first_token = data->first_token;
	data->node = recursive_parsing(data);
	free_tokens(first_token);
	
}

void	count_args_and_malloc(t_simple_cmd *cmd, t_token *token)
{
	t_token	*temporary;
	int		i;

	temporary = token;
	i = 0;
	while(temporary)
	{
		if (temporary->type == PIPE)
			break;
		if (temporary->type == STR || temporary->type == HEREDOC
		|| temporary->type == APPEND)
			i++;
		else if ((temporary->type == INPUT || temporary->type == OUTPUT) && temporary->next->type == STR)
			i--;
		temporary = temporary->next;
	}
	//printf("malloc arg size %d\n", i); //ok je dois capter le prob de mémoire ici
	cmd->args = malloc(sizeof(char *) * (i + 1));
	if (!cmd->args)
		EXIT_FAILURE;
}

t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
{
	t_simple_cmd	*cmd;
	int				i;

	//printf("first token of cmd : %s\n", token->content);
	i = 0;
	cmd = malloc(sizeof(t_simple_cmd));
	if (!cmd)
		EXIT_FAILURE;
	init_simple_cmd(cmd);
	count_args_and_malloc(cmd, token);
	while(token)
	{
		if (token->type == PIPE)
		{
			//ici aussi je vais devoir faire des trucs
			token = token->next;
			//printf("token after pipe : %s\n", token->content);
			break ;
		}
		else if (token->type == INPUT)
		{
			if (token->next != NULL)
				cmd->input = token->next->content;
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
		}
		else if (token->type == OUTPUT)
		{
			if (token->next != NULL)
				cmd->output = token->next->content;
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
		}
		else
		{
			if (i == 0)
				cmd->path_to_cmd = get_path(token->content, data->env);
			cmd->args[i] = ft_strdup(token->content);
			i++;
			token = token->next;
		}
	}
	cmd->args[i] = NULL;
	data->first_token = token;
	//printf("end of create cmd\n");
	return (cmd);
}