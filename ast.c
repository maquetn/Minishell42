/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <mdor@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:39:04 by mdor              #+#    #+#             */
/*   Updated: 2024/01/10 14:44:47 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

char	*get_env(char *name, char **env, t_minishell *data)
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
		path = ft_substr(env[i], 0, j, data);
		if (strcmp(path, name) == 0)
		{
			return (env[i] + j + 1);
		}
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env, t_minishell *data)
{
	char	*executable;
	int		i;
	char	**paths;
	char	*potential_path;

	i = -1;
	paths = ft_split(get_env("PATH", env, data), ':', data);
	if (!paths)
	{
		printf("split faild\n");
		exit(EXIT_FAILURE);//completer avec exit code correct
	}
	while (paths[++i])
	{
		potential_path = ft_strjoin(paths[i], "/", data);
		executable = ft_strjoin(potential_path, cmd, data);
		if (access(executable, F_OK | X_OK) == 0)
			{
				return(executable);
			}
	}
	return (ft_strdup(cmd, data));
}

void	add_file(char *name, t_minishell *data, t_files_list **head)
{
	t_files_list *newnode;
	t_files_list *temp = NULL;

	newnode = gc_malloc(sizeof(t_files_list), data);
	newnode->name = ft_strdup(name, data);
	newnode->next = NULL;

	if (*head == NULL)
		*head = newnode;
	else
	{
		temp = *head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = newnode;
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
	cmd->append_mode = 0;
	cmd->heredoc = 0;
}

t_simple_cmd	*get_cmd(t_token *token, t_minishell *data)
{
	t_simple_cmd	*cmd;
	
	cmd = create_simple_cmd(data, token);
	return (cmd);
}

t_simple_cmd	*recursive_parsing(t_minishell *data)
{
	t_simple_cmd	*prev = NULL;
	t_simple_cmd	*next = NULL;

	prev = get_cmd(data->first_token, data);
	if (data->first_token)
	{
		next = recursive_parsing(data);
		prev->next = next;
		next->prev = prev;
	}
	return (prev);
}

void	planting(t_minishell *data)
{
	// t_token	*first_token;
	
	// first_token = data->first_token;
	data->node = recursive_parsing(data);
}

void	count_args_and_malloc(t_simple_cmd *cmd, t_token *token, t_minishell *data)
{
	t_token	*temporary;
	int		i;

	temporary = token;
	i = 0;
	while(temporary)
	{
		if (temporary->type == PIPE)
			break;
		if (temporary->type == STR)
			i++;
		else if ((temporary->type == INPUT || temporary->type == OUTPUT
		|| temporary->type == HEREDOC || temporary->type == APPEND) && temporary->next != NULL && temporary->next->type == STR)
			i--;
		temporary = temporary->next;
	}
	if (i != 0)
		cmd->args = gc_malloc(sizeof(char *) * (i + 1), data);
	// if (!cmd->args)
	// 	EXIT_FAILURE;
}

t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
{
	t_simple_cmd	*cmd;
	int				i;

	i = 0;
	cmd = gc_malloc(sizeof(t_simple_cmd), data);
	if (!cmd)
		EXIT_FAILURE;
	init_simple_cmd(cmd);
	count_args_and_malloc(cmd, token, data);
	while(token)
	{
		if (token->type == PIPE)
		{
			if (token->next == NULL || token->next->type == PIPE)
				data->error_trigger = print_syntax_error(token);
			token = token->next;
			break ;
		}
		else if (token->type == HEREDOC)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == HEREDOC || token->next->type == INPUT || token->next->type == OUTPUT || token->next->type == APPEND)
			{
				data->error_trigger = print_syntax_error(token);
				if (token->next != NULL)
					token = token->next->next;
				else
					token = token->next;
				break;
			}
			cmd->heredoc_string = manage_heredoc(token->next->content, data);
			token = token->next->next;
			cmd->heredoc = 1;
		}
		else if (token->type == APPEND)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == HEREDOC || token->next->type == INPUT || token->next->type == OUTPUT || token->next->type == APPEND)
			{
				data->error_trigger = print_syntax_error(token);
				if (token->next != NULL)
					token = token->next->next;
				else
					token = token->next;
				break;
			}
			add_file(token->next->content, data, &cmd->output);
			token = token->next->next;
			cmd->append_mode = 1;
		}
		else if (token->type == INPUT)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == APPEND || token->next->type == OUTPUT)
			{
				data->error_trigger = print_syntax_error(token);
				if (token->next != NULL)
					token = token->next->next;
				else
					token = token->next;
				break;
			}
			add_file(token->next->content, data, &cmd->input);
			token = token->next->next;
			cmd->heredoc = 0;
		}
		else if (token->type == OUTPUT)
		{
			if (token->next == NULL || token->next->type == PIPE || token->next->type == INPUT || token->next->type == HEREDOC)
			{
				data->error_trigger = print_syntax_error(token);
				if (token->next != NULL)
					token = token->next->next;
				else
					token = token->next;
				break;
			}
			add_file(token->next->content, data, &cmd->output);
			token = token->next->next;
			cmd->append_mode = 0;
		}
		else
		{
			if (i == 0)
				cmd->path_to_cmd = get_path(token->content, data->env, data);
			if (cmd->args)
				cmd->args[i] = ft_strdup(token->content, data);
			i++;
			token = token->next;
		}
		if (data->error_trigger == 1)
			break;//manage error
	}
	if (cmd->args)
		cmd->args[i] = NULL;
	data->first_token = token;
	return (cmd);
}