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
char    *ft_strndup(char *str, int start, int end)
{
    int len;
    char    *dup;
    int     i;

    len = end - start + 1;
    i = 0;
    dup = malloc(sizeof(char) * (len + 1));
    while (i < len)
    {
        dup[i] = str[start + i];
        i++;
    }
    dup[len] = '\0';
    return (dup);
}


void add_token(t_token **head, t_token_type type, char *content, int i)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (new_token == NULL)
	{
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_token->type = type;
    new_token->content = strdup(content); // Duplicate the content
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
    if (i == 1)
        free(content);
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
		if (str[start] == '>' || str[start] == '<' || str[start] == '|' || str[start] == ' ' || str[start] == '$' || str[start] == '"' || str[start] == '\'')
		{
			//printf("what do I return in get next token %c i  : %d\n", str[start], start);
			return (start);
		}
		start++;
	}
	return (start);
}

int	get_spc_chr(char *str, int start)
{
	while(str[start])
	{

		if (str[start] == '>' || str[start] == '<' || str[start] == '|' || str[start] == ' ' || str[start] == '$' || str[start] == '"' || str[start] == '\'')
		{
			return (start - 1);
		}
		start++;
	}
	return (start);
}

int	get_correct_index(char *str, int i, int cas)
{
	if (cas == 1)
		i++;
	else if (cas == 2)
		i += 2;
	else if (cas == 3)
	{
		i++;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	return (i);
}


int	translate_dollar(char *str, int i, char **content, t_minishell *data)
{
	pid_t 	pid;
	char 	*placeholder;
	char	*translated;
	int		cas;

	pid = 0;
    cas = 1;
	if (str[i] == '$' && str[i + 1] == '$')
	{
		pid = getpid();
		translated = ft_itoa(pid);
		*content = ft_strjoin(*content, translated);
		free(translated);
		cas = 2;
	}
	if (str[i] == '$' && str[i + 1] == '?')
	{
		cas = errno;
		translated = ft_itoa(cas);
		*content = ft_strjoin(*content, translated);
		free(translated);
		cas = 2;
	}
	else if (str[i] == '$' && (str[i + 1] == '"' || str[i + 1] == '\''))
		return (i + 1);
	else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
	{
		placeholder = ft_strndup(str, i + 1, get_next_token(str, i + 1) - 1);
		translated = get_env(placeholder, data->env);
		if (translated == NULL)
			translated = ft_strdup("");
		*content = ft_strjoin(*content, translated);
		free(placeholder);
		cas = 3;
	}
	else if (str[i] == '$')
	{
		*content = ft_strjoin(*content, "$");
		cas = 1;
	}
	i = get_correct_index(str, i, cas);
	return (i);
}

int open_quote(char *str, int i, char quote) 
{
    int quotes_amount;

    quotes_amount = 0;
    while (str[i])
    {
        if (str[i] == quote)
            quotes_amount++;
        i++;
    }
    if (quotes_amount % 2 == 0)
        return (0);
    else
        return (1);
}

int	manage_double_quotes(char *str, int i, char **content, t_token **head)
{
	int		start;

	start = i + 1;
	while (str[i])
	{
		//reste alors que pas entre quotes
		// if (open_quote(str, i, '"'))
		// 	return (i + 1);
		i++;
		if (str[i] == '$')
		{
			*content = ft_strjoin(*content, ft_strndup(str, start, i - 1));
			break;
		}
		else if (str[i] == '"')
		{
			*content = ft_strjoin(*content, ft_strndup(str, start, i - 1));
			break;
		}
	}
    if (str[i + 1] == '\0')
        add_token(head, STR, *content, 1);
	// printf("content when ret \" : %s\n", *content);
	// printf("what do i ret %c & -1 %c & + 1 %c \n", str[i], str[i - 1], str[i + 1]);
	return (i);
}

int	manage_single_quotes(char *str, int i, char **content, t_token **head)
{
	int		start;

	start = i + 1;
	while (str[i])
	{
		//reste alors que pas entre quotes
		// if (open_quote(str, i, '\''))
		// 	return (i + 1);
		i++;
		if (str[i] == '\'')
		{
			*content = ft_strjoin_free2(*content, ft_strndup(str, start, i - 1));
            printf("content : %s\n", *content);
			break;
		}
	}
	// printf("content when ret \" : %s\n", *content);
	printf("what do i ret %c & -1 %c & + 1 %c et i : %d\n", str[i], str[i - 1], str[i + 1], i);
    if (str[i + 1] == '\0')
        add_token(head, STR, *content, 1);
	return (i);
}

int	something_before(char *str, int i)
{
	if (i == 0)
	{
		printf("syntax error\n");
		exit(EXIT_FAILURE);
	}
	i--;
	while (str[i] == ' ' || ft_isalnum(str[i]) || str[i] == '_')
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
			return (1);
		i--;
	}
	printf("syntax error\n");
	exit(EXIT_FAILURE);

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
int	manage_output_append(char *str, int i, t_token **head)
{
	if (str[i + 1] == '>' && something_behind(str, i + 1)) //rajouter un check qu'il y ai une redi possible sinon syntax error;
	{
		i++;
		add_token(head, APPEND, ">>", 0);
	}
	else if (something_behind(str, i))
		add_token(head, OUTPUT, ">", 0);
	return (i);
}

int	manage_input_heredoc(char *str, int i, t_token **head)
{
	if (str[i + 1] == '<' && something_behind(str, i + 1))
	{
		i++;
		add_token(head, HEREDOC, "<<", 0);
	}
	else if (something_behind(str, i))
		add_token(head, INPUT, "<", 0);
	return (i);
}

int	manage_pipe(int i, t_token **head, char *str)
{
	if (something_before(str, i))
		add_token(head, PIPE, "|", 0);
	return (i);
}

int	manage_dollar(char *str, int i, char **content, t_minishell *data)
{
	while (str[i] != '\0' && str[i] == '$')
	{
		i = translate_dollar(str, i, content, data);
		if (str[i] == '$')
			continue;
	}
	return (i);
}

void	token(char *str, t_minishell *data)
{
	t_token	*head = NULL;
	int		i;
	char	*content;
	int		dollar_quotes;

	i = 0;
	dollar_quotes = 0;
	content = ft_strdup("");
	while (str[i] != '\0')
	{
		//printf("\nchar : %c i being : %d surounded by : %c and %c\n\n", str[i], i, str[i - 1], str[i + 1]);
		if (str[i] == ' ')
		{
            //printf("when\n");
			i++;
			continue;
		}
		else if (str[i] == '$')
		{
            //printf("how do i get here : %d\n", i);
			dollar_quotes = 1;
			i = manage_dollar(str, i , &content, data);
		}
		else if (str[i] == '"')
		{
			if (open_quote(str, i, '"') == 1)
			{
				i++;
				continue;
			}
			i = manage_double_quotes(str, i, &content, &head);
			dollar_quotes = 1;
		}
		else if (str[i] == '\'')
		{
            //printf("hello\n");
			if (open_quote(str, i, '\'') == 1)
			{
				i++;
				continue;
			}
            //printf("hello\n");
			i = manage_single_quotes(str, i, &content, &head);
			dollar_quotes = 1;
		}
		else if (str[i] == '>')
			i = manage_output_append(str, i, &head) + 1;
		else if (str[i] == '<')
			i = manage_input_heredoc(str, i, &head) + 1;
		else if (str[i] == '|')
			i = manage_pipe(i , &head, str) + 1;
		else
		{
			content = ft_strjoin(content, ft_strndup(str, i, get_spc_chr(str, i)));
			//printf("what str[i] : %c et i = %d\n", str[i], i);
			i = get_next_token(str, i);
			//printf("contnet : %s\n", content);
			dollar_quotes = 2;
			//printf("char : %c i being : %d surounded by : %c and %c\n", str[i], i, str[i - 1], str[i + 1]);
			if(str[i] == '$' || str[i] == '\'' || str[i] == '"')
			{
				//printf("char : %c i being : %d surounded by : %c and %c\n", str[i], i, str[i - 1], str[i + 1]);
				continue;
			}
		}
		if(dollar_quotes == 1 || dollar_quotes == 2)
		{
            if (dollar_quotes == 1 && str[i + 1] != ' ' && str[i + 1] != '\0')
                continue;
			if(str[i] == '$' || str[i] == '\'' || str[i] == '"')
			{
				printf("char : %c i being : %d surounded by : %c and %c\n", str[i], i, str[i - 1], str[i + 1]);
				if (str[i + 1] == '\0' && str[i] == '"')
					add_token(&head, STR, content, 0);
				continue;
			}
			//printf("content %s , i : %d\n", content, i);
			add_token(&head, STR, content, 0);
			free(content);
			dollar_quotes = 0;
			content = ft_strdup("");
			//printf("%d\n", i);
		}
		//printf("hello\n");
		//printf("char : %c i being : %d surounded by : %c and %c\n", str[i], i, str[i - 1], str[i + 1]);
	}
	data->first_token = head;
	print_tokens(head);
}