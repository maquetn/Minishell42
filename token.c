#include "minishell.h"

void	free_tabl(char **tabl)
{
	int	i;

	i = 0;
	while (tabl[i])
	{
		free(tabl[i]);
		i++;
	}
	free(tabl);
}
char    *ft_strndup(char *str, int start, int end, t_minishell *data)
{
    int len;
    char    *dup;
    int     i;

    len = end - start + 1;
    i = 0;
    dup = gc_malloc(sizeof(char) * (len + 1), data);
    while (i < len)
    {
        dup[i] = str[start + i];
        i++;
    }
    dup[len] = '\0';
    return (dup);
}


void add_token(t_token **head, t_token_type type, char *content, t_minishell *data)
{
	if (content[0] == '\0')
		return;
    t_token *new_token = gc_malloc(sizeof(t_token), data);
    if (new_token == NULL)
	{
        perror("malloc");
		data->error_trigger = 1;
		return ;
    }

    new_token->type = type;
    new_token->content = ft_strdup(content, data);
    new_token->next = NULL;
    new_token->prev = NULL;

    if (*head == NULL) 
        *head = new_token;
	else 
	{
        t_token *current = *head;
        while (current->next != NULL) 
			current = current->next;
		current->next = new_token;
        new_token->prev = current;
    }
}

void    print_tokens(t_token *head)
{
	printf("these are the tokens : \n\n");
    while (head)
    {
        printf("type : %u content : %s\n", head->type, head->content);
        head = head->next;
    }
	printf("\nend of token display\n\n");
}


int get_next_token(char *str, int start)
{
	while(str[start])
	{
		if (str[start] == '>' || str[start] == '<' || str[start] == '|' || str[start] == ' ' || str[start] == '"' || str[start] == '\'' || str[start] == '\0')
		{
			return (start);
		}
		start++;
	}
	return (start - 1);
}

int	get_spc_chr(char *str, int start)
{
	while(str[start])
	{

		if (str[start] == '>' || str[start] == '<' || str[start] == '|' || str[start] == ' ' || str[start] == '"' || str[start] == '\'')
		{
			return (start - 1);
		}
		start++;
	}
	return (start);
}

int	manage_double_quotes(char *str, int i, char **content, t_minishell *data)
{
	int start;

	start = i + 1;
	while (str[i])
	{
		i++;
		if (str[i] == '\"' )
		{
			*content = ft_strjoin(*content, ft_strndup(str, start - 1, i, data), data);
			return (i);
		}
	}
	return (i);//si on arrive ici alors syntax error
}

int	manage_single_quotes(char *str, int i, char **content, t_minishell *data)
{
	int start;

	start = i + 1;
	while (str[i])
	{
		i++;
		if (str[i] == '\'' )
		{
			*content = ft_strjoin(*content, ft_strndup(str, start - 1, i, data), data);
			return (i);
		}
	}
	return (i);//si on arrive ici alors syntax error
}

int	something_behind(char *str, int i)
{
	i++;
	while(str[i])
	{
		if (str[i] != ' ' && str[i] != '\0')
			return (1);
		i++;
	}
	printf("syntax error \n");
	exit(EXIT_FAILURE);
	return (0);
}
int	manage_output_append(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i + 1] == '>')
	{
		i++;
		add_token(head, APPEND, ">>", data);
	}
	else
		add_token(head, OUTPUT, ">", data);
	return (i);
}

int	manage_input_heredoc(char *str, int i, t_token **head, t_minishell *data)
{
	if (str[i + 1] == '<')
	{
		i++;
		add_token(head, HEREDOC, "<<", data);
	}
	else
		add_token(head, INPUT, "<", data);
	return (i);
}

int	manage_pipe(int i, t_token **head, t_minishell *data)
{
	add_token(head, PIPE, "|", data);
	return (i);
}

int	not_special_char(char c)
{
	if (c == '"' || c == '\'' || c == '>' || c == '<' || c == '|' || c == ' ')
		return (0);
	else
		return (1);
}

void	token(char *str, t_minishell *data)
{
	int		i;
	char	*content;
	t_token	*head = NULL; 

	i = 0;
	content = ft_strdup("", data);
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
		{
			if (str[i - 1] == '"' || str[i - 1] == '\'')
			{
				add_token(&head, STR, content, data);
				content = ft_strdup("", data);
			}
			i++;
			continue;
		}
		else if (str[i] == '>')
		{
			i = manage_output_append(str, i, &head, data) + 1;
			continue;
		}
		else if (str[i] == '<')
		{
			i = manage_input_heredoc(str, i, &head, data) +1;
			continue;
		}
		else if (str[i] == '|')
		{
			i = manage_pipe(i, &head, data) + 1;
			continue;
		}
		else if (str[i] == '\'')
			i = manage_single_quotes (str, i, &content, data);
		else if (str[i] == '"')
			i = manage_double_quotes(str, i, &content, data);
		if (str[i] == '\'' || str[i] == '"')
		{
			i++;
			if (str[i] == '\0')
				add_token(&head, STR, content, data);
			continue;
		}
		else
		{
			content = ft_strjoin(content, ft_strndup(str, i, get_spc_chr(str, i), data), data);
			i = get_next_token(str, i);
			if (str[i] != '\'' && str[i] != '"')
			{
				add_token(&head, STR, content, data);
				content = ft_strdup("", data);
			}
			if (str[i + 1] == '\0')
				break;
		}
	}
	data->first_token = head;
	//print_tokens(head);
}