// t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token)
// {
// 	t_simple_cmd	*cmd;
// 	int				i;

// 	i = 0;
// 	cmd = gc_malloc(sizeof(t_simple_cmd), data);
// 	if (!cmd)
// 		EXIT_FAILURE;
// 	init_simple_cmd(cmd);
// 	malloc_args(cmd, token, data);
// 	while(token)
// 	{
// 		if (token->type == PIPE)
// 		{
// 			if (token->next == NULL || token->next->type == PIPE)
// 				data->error_trigger = print_syntax_error(token);
// 			token = token->next;
// 			break ;
// 		}
// 		else if (token->type == HEREDOC)
// 		{
// 			if (token->next == NULL || token->next->type == PIPE || token->next->type == HEREDOC || token->next->type == INPUT || token->next->type == OUTPUT || token->next->type == APPEND)
// 			{
// 				data->error_trigger = print_syntax_error(token);
// 				if (token->next != NULL)
// 					token = token->next->next;
// 				else
// 					token = token->next;
// 				break;
// 			}
// 			cmd->heredoc_string = manage_heredoc(token->next->content, data);
// 			token = token->next->next;
// 			cmd->heredoc = 1;
// 		}
// 		else if (token->type == APPEND)
// 		{
// 			if (token->next == NULL || token->next->type == PIPE || token->next->type == HEREDOC || token->next->type == INPUT || token->next->type == OUTPUT || token->next->type == APPEND)
// 			{
// 				data->error_trigger = print_syntax_error(token);
// 				if (token->next != NULL)
// 					token = token->next->next;
// 				else
// 					token = token->next;
// 				break;
// 			}
// 			add_file(token->next->content, data, &cmd->output);
// 			token = token->next->next;
// 			cmd->append_mode = 1;
// 		}
// 		else if (token->type == INPUT)
// 		{
// 			if (token->next == NULL || token->next->type == PIPE || token->next->type == APPEND || token->next->type == OUTPUT)
// 			{
// 				data->error_trigger = print_syntax_error(token);
// 				if (token->next != NULL)
// 					token = token->next->next;
// 				else
// 					token = token->next;
// 				break;
// 			}
// 			add_file(token->next->content, data, &cmd->input);
// 			token = token->next->next;
// 			cmd->heredoc = 0;
// 		}
// 		else if (token->type == OUTPUT)
// 		{
// 			if (token->next == NULL || token->next->type == PIPE || token->next->type == INPUT || token->next->type == HEREDOC)
// 			{
// 				data->error_trigger = print_syntax_error(token);
// 				if (token->next != NULL)
// 					token = token->next->next;
// 				else
// 					token = token->next;
// 				break;
// 			}
// 			add_file(token->next->content, data, &cmd->output);
// 			token = token->next->next;
// 			cmd->append_mode = 0;
// 		}
// 		else
// 		{
// 			if (i == 0)
// 				cmd->path_to_cmd = get_path(token->content, data->env, data);
// 			if (cmd->args)
// 				cmd->args[i] = ft_strdup(token->content, data);
// 			i++;
// 			token = token->next;
// 		}
// 		if (data->error_trigger == 1)
// 			break;//manage error
// 	}
// 	if (cmd->args)
// 		cmd->args[i] = NULL;
// 	data->first_token = token;
// 	return (cmd);
// }