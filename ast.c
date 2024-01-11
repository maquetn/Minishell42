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

#include "minishell.h"	

void	malloc_args(t_simple_cmd *cmd, t_token *token, t_minishell *data)
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
	if (!cmd->args)
		EXIT_FAILURE;
}

t_simple_cmd	*init_simple_cmd_wrapper(t_minishell *data, t_token *token)
{
	t_simple_cmd	*cmd;

	cmd = gc_malloc(sizeof(t_simple_cmd), data);
	if (!cmd)
		EXIT_FAILURE;
	init_simple_cmd(cmd);
	malloc_args(cmd, token, data);
	return (cmd);
}

void	other(t_minishell *data, t_token **token, t_simple_cmd *cmd, int *i)
{
	if (*i == 0)
		cmd->path_to_cmd = get_path((*token)->content, data->env, data);
	if (cmd->args)
		cmd->args[*i] = ft_strdup((*token)->content, data);
	(*i)++;
	*token = (*token)->next;
}

t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
{
	int				i;
	t_simple_cmd	*cmd;

	i = 0;
	cmd = init_simple_cmd_wrapper(data, token);
	while (token)
	{
		if (token->type == PIPE) 
		{
			process_pipe_token(data, &token);
			break ;
		}
		else if (token->type == HEREDOC)
			process_heredoc(data, &token, cmd);
		else if (token->type == APPEND)
			process_append(data, &token, cmd);
		else if (token->type == INPUT)
			process_input(data, &token, cmd);
		else if (token->type == OUTPUT)
			process_output(data, &token, cmd);
		else
			other(data, &token, cmd, &i);
		if (data->error_trigger != 0)
			break ;
	}
	if (cmd->args)
		cmd->args[i] = NULL;
	data->first_token = token;
	return (cmd);
}
