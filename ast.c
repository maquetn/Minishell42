/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:58:42 by nmaquet           #+#    #+#             */
/*   Updated: 2023/12/27 15:44:23 by nmaquet          ###   ########.fr       */
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

char *get_env_value(char *env_var, char **env)
{
    int i = 0;
    while (env[i] != NULL)
	{
        if (strncmp(env[i], env_var, strlen(env_var)) == 0 && env[i][strlen(env_var)] == '=')
		{
            return env[i] + strlen(env_var) + 1;
        }
        i++;
    }
    return NULL;
}

char *expand_env_variables(char *str, char **env)
{
    char *result = strdup(str);
    if (result == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    char *start = result;
    while ((start = strchr(start, '$')) != NULL)
    {
        char *end = strchr(start + 1, ' ');
        if (end == NULL)
        {
            end = start + strlen(start);
        }
        char *env_var = strndup(start + 1, end - start - 1);
        char *env_value = get_env_value(env_var, env);
        size_t prefix_len = start - result;
        size_t suffix_len = strlen(end);

        if (env_value != NULL)
        {
            size_t expanded_len = prefix_len + strlen(env_value) + suffix_len;
            char *expanded = malloc(expanded_len + 1);
            if (expanded == NULL)
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            strncpy(expanded, result, prefix_len);
            strcpy(expanded + prefix_len, env_value);
            strcpy(expanded + prefix_len + strlen(env_value), end);

            free(result);
            result = expanded;
            start = expanded + prefix_len + strlen(env_value);
        }
        else
        {
            // Env var doenst exist
            *start = '\0';
            start += 1;
            result = realloc(result, strlen(result) + 1);
            if (result == NULL)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        free(env_var);
    }
    return result;
}


char *get_path(char *cmd, char **env)
{
    char *expanded_cmd = expand_env_variables(cmd, env);
    
    char *executable;
    int i;
    char **paths;
    char *potential_path;

    i = -1;
    paths = ft_split(get_env("PATH", env), ':');
    if (!paths)
        EXIT_FAILURE;

    while (paths[++i])
	{
        potential_path = ft_strjoin(paths[i], "/");
        executable = ft_strjoin(potential_path, expanded_cmd);
        free(potential_path);

        if (access(executable, F_OK | X_OK) == 0)
		{
            free(expanded_cmd);
            free_tabl(paths);
            return executable;
        }
        free(executable);
    }
    free(expanded_cmd);
    free_tabl(paths);
    return strdup(cmd);
}

void free_tokens(t_token *token)
{
	t_token *temp;
	t_token *next_token;

	temp = token;
	while (temp)
	{
		free(temp->content);
		next_token = temp->next;
		temp->prev = NULL;
		free(temp);
		temp = next_token;
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
	t_simple_cmd	*next = NULL;

	prev = get_cmd(data->first_token, data);

	if (data->first_token)
	{
		next = recursive_parsing(data);
		prev->next = next;
		next->prev = prev;
	}

	return prev;
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
			break ;
		}
		else if (token->type == INPUT)
		{
			if (token->next != NULL)
				cmd->input = ft_strdup(token->next->content);
			if (token->next != NULL && token->next->type != OUTPUT && token->next->type != INPUT)
				token = token->next->next;
			else
				token = token->next;
		}
		else if (token->type == OUTPUT)
		{
			if (token->next != NULL)
				cmd->output = ft_strdup(token->next->content);
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
	return (cmd);
}
