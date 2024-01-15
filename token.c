/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   token.c											:+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/14 18:20:07 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/14 18:20:08 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "minishell.h"	

int	inp_out_pipe(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i] == '>')
		i = manage_output_append(str, i, head, data) + 1;
	else if (str[i] == '<')
		i = manage_input_heredoc(str, i, head, data) + 1;
	else if (str[i] == '|')
		i = manage_pipe(i, head, data) + 1;
	return (i);
}

int	quotes(char *str, int i, char **content, t_minishell *data)
{
	if (str[i] == '\'')
		i = manage_single_quotes (str, i, content, data);
	else if (str[i] == '"')
		i = manage_double_quotes(str, i, content, data);
	return (i);
}

void	add_after_space(char **c, t_token **head, t_minishell *data)
{
	add_token(head, STR, *c, data);
	*c = ft_strdup("", data);
}

int	add_normal_str(char	*str, int i, char **content, t_minishell *data)
{
	*content = ft_strjoin(*content,
			ft_strndup(str, i, get_spc_chr(str, i), data), data);
	return (i = get_next_token(str, i));
}

void	add_token_str(t_token **head, char **content, t_minishell *data)
{
	add_token(head, STR, *content, data);
	*content = ft_strdup("", data);
}

	//print_tokens(head);
// void	token(char *str, t_minishell *data)
// {
// 	int		i;
// 	char	*content;
// 	t_token	*head; 

// 	i = 0;
// 	head = NULL;
// 	content = ft_strdup("", data);
// 	while (str[i] && data->error_trigger == 0 && i < ft_strlen(str))
// 	{
// 		if (str[i] == ' ')
// 		{
// 			if (i > 0 && (str[i - 1] == '"' || str[i++ - 1] == '\''))
// 				add_after_space(&content, &head, data);
// 			continue ;
// 		}
// 		else if (str[i] == '>' || str[i] == '<' || str[i] == '|')
// 		{
// 			i = inp_out_pipe(str, i, &head, data);
// 			continue ;
// 		}
// 		else if (str[i] == '\'' || str[i] == '"')
// 			i = quotes(str, i, &content, data);
// 		if (str[i] == '\'' || str[i] == '"')
// 		{
// 			if (str[++i] == '\0')
// 				add_token(&head, STR, content, data);
// 		}
// 		else
// 		{
// 			i = add_normal_str(str, i, &content, data);
// 			if (str[i] != '\'' && str[i] != '"')
// 				add_token_str(&head, &content, data);
// 			if (str[i + 1] == '\0' || data->error_trigger != 0)
// 				break ;
// 		}
// 	}
// 	data->first_token = head;
// }
