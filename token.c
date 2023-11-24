#include "minishell.h"


// Function to trim leading and trailing spaces from a string
void trim(char *str)
{
	int start = 0, end = ft_strlen(str) - 1;

	// Trim leading spaces
	while (str[start] == ' ')
	{
		start++;
	}

	// Trim trailing spaces
	while (end > start && str[end] == ' ')
	{
		end--;
	}

	// Shift characters to the beginning of the string
	for (int i = 0; i <= end - start; ++i)
	{
		str[i] = str[start + i];
	}

	// Null-terminate the trimmed string
	str[end - start + 1] = '\0';
}

int token(char *input)
{
	char *tokens[100];
	int tokenCount = 0;
	int i = 0;
	int j = 0;

	while (input[i])
	{
		if (input[i] == '|' || input[i] == '>' || input[i] == '<' || input[i] == ' ')
		{
			if (j > 0)
			{
				tokens[tokenCount] = malloc(j + 1);
				ft_strncpy(tokens[tokenCount], input + i - j, j);
				tokens[tokenCount][j] = '\0';
				trim(tokens[tokenCount]);
				printf("Token: %s\n", tokens[tokenCount]);
				tokenCount++;
				j = 0;
			}

			// Nouveau token pour le délimiteur ou l'espace
			if (input[i] != ' ')
			{
				tokens[tokenCount] = malloc(2);
				tokens[tokenCount][0] = input[i];
				tokens[tokenCount][1] = '\0';

				// Si deux délimiteurs se suivent, les concaténer dans un seul token
				if ((input[i] == '<' || input[i] == '>') && input[i + 1] == input[i])
				{
					tokens[tokenCount][1] = input[i + 1];
					tokens[tokenCount][2] = '\0';
					i++; // Avancer d'un caractère supplémentaire
				}

				printf("Delimiter: %s\n", tokens[tokenCount]);
				tokenCount++;
			}
		}
		else
			j++;
		i++;
	}

	if (j > 0)
	{
		tokens[tokenCount] = malloc(j + 1);
		ft_strncpy(tokens[tokenCount], input + ft_strlen(input) - j, j);
		tokens[tokenCount][j] = '\0';
		trim(tokens[tokenCount]);
		printf("Token: %s\n", tokens[tokenCount]);
		tokenCount++;
	}

	i = 0;
	while (i < tokenCount)
	{
		free(tokens[i]);
		++i;
	}
	return (0);
}

