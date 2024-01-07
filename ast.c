/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <mdor@student.s19.be>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 09:39:04 by mdor              #+#    #+#             */
/*   Updated: 2023/12/30 13:55:38 by mdor             ###   ########.fr       */
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
		EXIT_FAILURE;
	while (paths[++i])
	{
		potential_path = ft_strjoin(paths[i], "/", data);
		executable = ft_strjoin(potential_path, cmd, data);
		if (access(executable, F_OK | X_OK) == 0)
			{
				return(executable);
			}
	}
	return (strdup(cmd));
}

// void free_tokens(t_token *token)
// {
//     t_token *temp;
//     t_token *next_token;

//     temp = token;
//     while (temp)
//     {
//         //printf("Freeing token: %p, content: %s\n", (void *)temp, temp->content);

//         free(temp->content);

//         next_token = temp->next;
//         temp->prev = NULL;
//         free(temp);

//         temp = next_token;
//     }
// }


void	init_simple_cmd(t_simple_cmd *cmd)
{
	cmd->path_to_cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->prev = NULL;
    cmd->next = NULL;
	cmd->heredoc_tabl = NULL;
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
	t_token	*first_token;
	
	first_token = data->first_token;
	//printf("%s\n", first_token->content);
	data->node = recursive_parsing(data);
	//printf("%s\n", first_token->content);
	//free_tokens(first_token);
}

void	count_args_and_malloc(t_simple_cmd *cmd, t_token *token, t_minishell *data)
{
	t_token	*temporary;
	int		i;

	temporary = token;
	//printf("Content: %s \n", temporary->content);
	i = 0;
	while(temporary)
	{
		//printf("while\n");
		if (temporary->type == PIPE)
			break;
		if (temporary->type == STR || temporary->type == HEREDOC
		|| temporary->type == APPEND)
			i++;
		else if ((temporary->type == INPUT || temporary->type == OUTPUT) && temporary->next->type == STR)
			i--;
		//printf("haha\n");
		temporary = temporary->next;
		//printf("?\n");
	}
	//printf("malloc arg size %d\n", i); //ok je dois capter le prob de mémoire ici
	cmd->args = gc_malloc(sizeof(char *) * (i + 1), data);
	if (!cmd->args)
		EXIT_FAILURE;
}

t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
{
	t_simple_cmd	*cmd;
	int				i;

	//printf("first token of cmd : %s\n", token->content);
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
			token = token->next;
			break ;
		}
		else if (token->type == HEREDOC)
		{
			//faire en sorte que mon noeud soit un heredoc
			if (token->next != NULL)
				cmd->input = ft_strdup(token->next->content, data);
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
			cmd->heredoc = 1;
		}
		else if (token->type == APPEND)
		{
			if (token->next != NULL)
				cmd->output = ft_strdup(token->next->content, data);
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
			cmd->append_mode = 1;
		}
		else if (token->type == INPUT)
		{
			if (token->next != NULL)
				cmd->input = ft_strdup(token->next->content, data);
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
		}
		else if (token->type == OUTPUT)
		{
			if (token->next != NULL)
				cmd->output = ft_strdup(token->next->content, data);
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
		}
		else
		{
			if (i == 0)
				cmd->path_to_cmd = get_path(token->content, data->env, data);
			cmd->args[i] = ft_strdup(token->content, data);
			i++;
			token = token->next;
		}
	}
	cmd->args[i] = NULL;
	data->first_token = token;
	return (cmd);
}