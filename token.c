#include "minishell.h"

void trim(char *str)
{
	int start = 0, end = ft_strlen(str) - 1;

	while (str[start] == ' ')
	{
		start++;
	}

	while (end > start && str[end] == ' ')
	{
		end--;
	}

	for (int i = 0; i <= end - start; ++i)
	{
		str[i] = str[start + i];
	}

	str[end - start + 1] = '\0';
}


int token(char *input)
{
	char *tokens[100];
	int content = 0;
	int i = 0;
	int j = 0;

	while (input[i])
	{
		if (input[i] == '|' || input[i] == '>' || input[i] == '<' || input[i] == ' ' || input[i] == '\0')

		{
			if (j > 0)
			{
				tokens[content] = malloc(j + 1);
				strncpy(tokens[content], input + i - j, j);
				tokens[content][j] = '\0';
				trim(tokens[content]);
				printf("Token: %s\n", tokens[content]);
				content++;
				j = 0;
			}

			if (input[i] != ' ')
			{
				tokens[content] = malloc(2);
				tokens[content][0] = input[i];
				tokens[content][1] = '\0';

				if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
				{
					tokens[content][1] = input[i + 1];
					tokens[content][2] = '\0';
					i++;
				}

				printf("Delimiter: %s\n", tokens[content]);
				content++;
			}
		}
		else
			j++;
		i++;
	}

	if (j > 0)
	{
		tokens[content] = strndup(input + i - j, j);
		trim(tokens[content]);
		printf("Token: %s\n", tokens[content]);
		content++;
	}

	i = -1;
	while (++i < content)
	{
		free(tokens[i]);
	}
	return (0);
}

