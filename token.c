#include "minishell.h"

void expand_path_in_tokens(char **input) 
{
	char *expanded_path = expand_path(*input);
	*input = expanded_path;
}

void trim(char *str) 
{
	int start = 0, end = ft_strlen(str) - 1;

	while (str[start] == ' ') 
		start++;
	while (end > start && str[end] == ' ') 
		end--;
	for (int i = 0; i <= end - start; ++i) 
		str[i] = str[start + i];
	str[end - start + 1] = '\0';
}

int token(char *input) 
{
	t_token *tokens[100];
	int arg = 0;
	int i = 0;
	int j = 0;

	//char* current_directory = get_current_directory();
	//expand_path_in_tokens(&current_directory);

	//expand_path_in_tokens(&input);
	while (input[i]) 
{
	if (input[i] == '|' || input[i] == '>' || input[i] == '<' || input[i] == ' ' || input[i] == '\0') 
	{
		if (j > 0) 
		{
			tokens[arg] = (t_token *)malloc(sizeof(t_token));
			tokens[arg]->content = strndup(input + i - j, j);
			trim(tokens[arg]->content);
			check_type(tokens, arg);
			//printf("Token: %s\n", tokens[arg]->content);
			if (tokens[arg]->type == CMD) 
			{
				//try_executing(tokens[arg]->content, get_path(), tokens[arg]->content);
			}
			arg++;
			j = 0;
		}

		if (input[i] != ' ') 
		{
			tokens[arg] = (t_token *)malloc(sizeof(t_token));
			if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i]) 
			{
				tokens[arg]->content = malloc(3);
				tokens[arg]->content[0] = input[i];
				tokens[arg]->content[1] = input[i + 1];
				tokens[arg]->content[2] = '\0';
				i++;
			} 
			else 
			{
				tokens[arg]->content = malloc(2);
				tokens[arg]->content[0] = input[i];
				tokens[arg]->content[1] = '\0';
			}
			check_type(tokens, arg);
			//printf("Delimiter: %s\n", tokens[arg]->content);
			arg++;
		}
	} 
	else 
	{
		j++;
	}

	i++;
}

	if (j > 0) 
	{
		tokens[arg] = (t_token *)malloc(sizeof(t_token));
		tokens[arg]->content = strndup(input + i - j, j);
		trim(tokens[arg]->content);
		check_type(tokens, arg);

		if (tokens[arg]->type == CMD) 
		{
			exec_builtin(tokens[arg]->content, *tokens, arg);
			try_executing(tokens[arg]->content, get_path());
		}
		//printf("Token: %s\n", tokens[arg]->content);
		arg++;
	}

	//free(current_directory);
	i = 0;
	while (i < arg) 
	{
		free(tokens[i]->content);
		free(tokens[i]);
		++i;
	}

	return 0;
}
