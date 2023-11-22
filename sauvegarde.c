#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to trim leading and trailing spaces from a string
void trim(char *str)
{
	int start = 0, end = strlen(str) - 1;

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

int main()
{
	char *str = "   he>l >lo world >";
	char *tokens[100];
	int tokenCount = 0;
	int j = 0;

	for (int i = 0; str[i]; ++i)
	{
		if (str[i] == '|' || str[i] == '>' || str[i] == '<' || str[i] == ' ')
		{
			if (j > 0)
			{
				tokens[tokenCount] = malloc(j + 1);
				strncpy(tokens[tokenCount], str + i - j, j);
				tokens[tokenCount][j] = '\0';
				trim(tokens[tokenCount]);
				printf("Token: %s\n", tokens[tokenCount]);
				tokenCount++;
				j = 0;
			}

			// Nouveau token pour le délimiteur ou l'espace
			if (str[i] != ' ')
			{
				tokens[tokenCount] = malloc(2);
				tokens[tokenCount][0] = str[i];
				tokens[tokenCount][1] = '\0';
				printf("Delimiter: %c\n", str[i]);
				tokenCount++;
			}
		}
		else
		{
			j++;
		}
	}

	if (j > 0)
	{
		tokens[tokenCount] = malloc(j + 1);
		strncpy(tokens[tokenCount], str + strlen(str) - j, j);
		tokens[tokenCount][j] = '\0';
		trim(tokens[tokenCount]);
		printf("Token: %s\n", tokens[tokenCount]);
		tokenCount++;
	}

	int i = 0;
	while(i < tokenCount)
	{
		printf("\n%s", tokens[i]);
		i++;
	}
	for (int i = 0; i < tokenCount; ++i)
	{
		free(tokens[i]);
	}

	return 0;
}
