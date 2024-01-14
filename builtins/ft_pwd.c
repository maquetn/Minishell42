/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   ft_pwd.c										   :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/14 14:21:20 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/14 14:21:25 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_pwd(void)
{
	char	*cwd;

	cwd = malloc(PATH_MAX);
	if (cwd == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		free(cwd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(cwd, "/") == 0)
	{
		free(cwd);
		return (strdup("/"));
	}
	return (cwd);
}
