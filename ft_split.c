#include "minishell.h"

int	ft_strlen(const char *str)
{
	size_t	c;

	if (!str)
		return (0);
	c = 1;
	while (str[c] != '\0')
		c++;
	return (c);
}
