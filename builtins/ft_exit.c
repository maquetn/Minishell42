/* ************************************************************************** */
/*																			  */
/*														:::	  ::::::::        */
/*   ft_exit.c										  :+:	  :+:	:+:       */
/*													+:+ +:+		 +:+	      */
/*   By: mdor <marvin@42.fr>						+#+  +:+	   +#+	      */
/*												+#+#+#+#+#+   +#+		      */
/*   Created: 2024/01/14 14:14:00 by mdor			  #+#	#+#			      */
/*   Updated: 2024/01/14 14:14:09 by mdor			 ###   ########.fr	      */
/*																			  */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(t_minishell *data)
{
	printf("exit");
	printf("\n");
	free_custom_alloc(data);
	free_tabl(data->env);
	free(data->input);
	exit(data->exit_code);
}
