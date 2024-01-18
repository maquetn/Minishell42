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

int		ft_exit_atoi(const char *str, bool *error)
{
	int neg;
	int i;
	int num;

	i = 0;
	neg = 1;
	num = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
		num = num * 10 + (str[i++] - 48);
	if (ft_isdigit(str[i]) == 0)
	{
 		error = (bool*)true;
		printf("helo\n");
		return (255);
	}
	return (num * neg);
}

bool	process_code_ex(char *str, t_minishell *data)
{
	int		i;
	bool	b;

	i = ft_exit_atoi(str, &b);
	if (b == true)
	{
		ft_putstr_fd("minishell: exit:", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	if (i != 255)
		i = i % 255;
	printf("i : %d\n", i);
	data->error_trigger = i;
	return (b);
}

void	ft_exit(t_minishell *data, t_simple_cmd *cmd, int i)
{
	int		j;
	bool	b;

	j = 0;
	b = false;
	if (i == 1)
		printf("exit\n");
	if (cmd != NULL)
		while (cmd->args[j])
			j++;
	if (j >= 2)
		b = process_code_ex(cmd->args[1], data);
	if (j > 2 || b == true)
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	if (i == 1)
	{
		free_custom_alloc(data);
		free(data->input);
		free_tabl(data->env);
		exit(data->error_trigger);//logique ici 
	}
}
