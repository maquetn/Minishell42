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
char	*ft_strndup(char *str, int start, int end)
{
	int len;
	char	*dup;
	int	 i;

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

int check_spc_chr(t_token **head, char *str)
{
	int i;
	int			 position;
	int			 spc;

	i = 0;
	position = 0;
	spc = 0;
	while (str[i])
	{
		if (str[i] == '>' && str[i + 1] == '>')
		{
			if (i >= 1 && str[i - 1] != '<')
				add_token(head, STR, ft_strndup(str, position, i - 1), 1);
			add_token(head, APPEND, ">>", 0);
			position = i + 2;
			spc = 1;
			i++;
		}
		else if (str[i] == '>')
		{
			if (i >= 1 && str[i - 1] != '<')
				add_token(head, STR, ft_strndup(str, position, i - 1), 1);
			add_token(head, OUTPUT, ">", 0);
			position = i + 1;
			spc = 1;
		}
		else if (str[i] == '<' && str[i + 1] == '<')
		{
			if (i >= 1 && str[i - 1] != '>')
				add_token(head, STR, ft_strndup(str, position, i - 1), 1);
			add_token(head, HEREDOC, "<<", 0);
			position = i + 2;
			spc = 1;
			i++;
		}
		else if (str[i] == '<')
		{
			if (i >= 1 && str[i - 1] != '>')
				add_token(head, STR, ft_strndup(str, position, i - 1), 1);
			add_token(head, INPUT, "<", 0);
			position = i + 1;
			spc = 1;
		}
		else if (str[i] == '|')
		{
			if (i >= 1)
				add_token(head, STR, ft_strndup(str, position, i - 1), 1);
			add_token(head, PIPE, "|", 0);
			position = i + 1;
			spc = 1;
		}
		i++;
	}
	if (spc == 1 && str[position])
	{
		//printf("times\n");
		//printf("position : %d char %d = %c et len str : %d\n", position, position, str[position], ft_strlen(str));
		add_token(head, STR, ft_strndup(str, position, (ft_strlen(str))), 1);
	}
	return (spc);
}

void add_token(t_token **head, t_token_type type, char *content, int i)
{
	t_token *new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
	{
		perror("malloc");
		ft_exit(EXIT_FAILURE, NULL);
	}

	new_token->type = type;
	new_token->content = strdup(content);
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

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("type : %u content : %s\n", head->type, head->content);
		head = head->next;
	}
}

void remove_quotes(char **str)
{
    int len = strlen(*str);
    int j = 0;
    int in_quotes = 0;

    for (int i = 0; i < len; ++i)
    {
        if ((*str)[i] == '\"' || (*str)[i] == '\'')
        {
            if (in_quotes && (*str)[i] == in_quotes)
                in_quotes = 0;
            else
                in_quotes = (*str)[i];
        }
        else
        {
            if (!in_quotes || (*str)[i] != in_quotes)
            {
                (*str)[j] = (*str)[i];
                j++;
            }
        }
    }

    (*str)[j] = '\0';
}



void token(char *input, t_minishell *data)
{
    t_token *head = NULL;
    char **splited_str;
    char *content;
    int i;
    t_token_type type;

    i = 0;
    splited_str = ft_split(input, ' ');
    while (splited_str[i])
    {
        content = strdup(splited_str[i]); // Duplicate the token content

        if (strcmp(content, "<") == 0)
            type = INPUT;
        else if (strcmp(content, ">") == 0)
            type = OUTPUT;
        else if (strcmp(content, ">>") == 0)
            type = APPEND;
        else if (strcmp(content, "<<") == 0)
            type = HEREDOC;
        else if (strcmp(content, "|") == 0)
            type = PIPE;
        else if (check_spc_chr(&head, content))
        {
            free(content);
            i++;
            continue;
        }
        else
            type = STR;

        remove_quotes(&content);

        add_token(&head, type, content, 0);
        free(content);
        i++;
    }
    data->first_token = head;
    free_tabl(splited_str);
}

