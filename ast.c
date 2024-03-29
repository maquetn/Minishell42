/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::		  */
/*   ast.c											  :+:	  :+:	:+:	      */
/*													+:+ +:+		 +:+		  */
/*   By: mdor <mdor@student.s19.be>				 +#+  +:+	   +#+			  */
/*												+#+#+#+#+#+   +#+			  */
/*   Created: 2023/12/08 09:39:04 by mdor			  #+#	#+#				  */
/*   Updated: 2024/01/10 14:44:47 by mdor			 ###   ########.fr		  */
/*																			  */
/* ************************************************************************** */

//malloc protected

#include "minishell.h"	

int	malloc_args(t_simple_cmd *cmd, t_token *token, t_minishell *data)
{
	t_token	*temporary;
	int		i;

	temporary = token;
	i = 0;
	while (temporary)
	{
		if (temporary->type == PIPE)
			break ;
		if (temporary->type == STR)
			i++;
		else if ((temporary->type == INPUT || temporary->type == OUTPUT
				|| temporary->type == HEREDOC || temporary->type == APPEND)
			&& temporary->next != NULL && temporary->next->type == STR)
			i--;
		temporary = temporary->next;
	}
	if (i != 0)
		cmd->args = gc_malloc(sizeof(char *) * (i + 1), data);
	if (cmd->args == NULL)
		return (1);
	return (0);
}

t_simple_cmd	*init_simple_cmd_wrapper(t_minishell *data, t_token *token)
{
	t_simple_cmd	*cmd;

	cmd = gc_malloc(sizeof(t_simple_cmd), data);
	if (cmd == NULL)
		return (NULL);
	init_simple_cmd(cmd);
	if (malloc_args(cmd, token, data))
		return (NULL);
	return (cmd);
}

void	other(t_minishell *data, t_token **token, t_simple_cmd *cmd, int *i)
{
	if ((*token)->content[0] == '\0')
	{
		*token = (*token)->next;
		return ;
	}
	if (*i == 0)
		cmd->path_to_cmd = get_path((*token)->content, data->env, data);
	if (cmd->args)
		cmd->args[*i] = ft_strdup((*token)->content, data);
	if (cmd->args[*i] == NULL || cmd->path_to_cmd == NULL)
	{
		data->error_trigger = 1;
		return ;
	}
	(*i)++;
	*token = (*token)->next;
	return ;
}

void	process_all(t_token **token, t_minishell *data,
	t_simple_cmd *cmd, int *i)
{
	if ((*token)->type == HEREDOC)
		process_heredoc(data, token, cmd);
	else if ((*token)->type == APPEND)
		process_append(data, token, cmd);
	else if ((*token)->type == INPUT)
		process_input(data, token, cmd);
	else if ((*token)->type == OUTPUT)
		process_output(data, token, cmd);
	else
		other(data, token, cmd, i);
}

t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
{
	int				i;
	t_simple_cmd	*cmd;

	i = 0;
	cmd = init_simple_cmd_wrapper(data, token);
	if (cmd == NULL)
		return (NULL);
	while (token)
	{
		if (token->type == PIPE) 
		{
			process_pipe_token(data, &token);
			break ;
		}
		else
			process_all(&token, data, cmd, &i);
		if (data->error_trigger != 0)
			break ;
	}
	if (cmd->args)
		cmd->args[i] = NULL;
	data->first_token = token;
	if (data->error_trigger != 0)
		data->first_token = NULL;
	return (cmd);
}
