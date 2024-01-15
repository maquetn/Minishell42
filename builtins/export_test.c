#include "../minishell.h"

int	get_name(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '+' && str[i] != '=')
		i++;
	return (i);
}

char	**create_env_copy(char **env, t_minishell *data, char *new_var, int exist)
{
	char	**new_env;

	
}

void	append_var(t_minishell *data, char *new_var, char **env)
{
	int		env_size;
	char	**new_env;
	int		var_name_len;
	int		exists;

	var_name_len = get_name(new_var);
	env_size = 0;
	new_env = NULL;
	exists = 0;
	while (env[env_size])
	{
		if (ft_strncmp(env[env_size], new_env, var_name_len) == 0)
			exists = 1;
		env_size++;
	}
	if (exists == 1)
		env_size--;
	new_env = create_env_copy(env_size, env, new_var, exists);
}


void	count_new_vars(t_minishell *data, char **args)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	while (args[i] != NULL)
	{
		j = 0;
		while (args[i][j] != '\0' && (args[i][j] != '+' || args[i][j] != '='))
		{
			if (ft_isalnum(args[i][j]) == 0 && args[i][j] != '_')
				break ;
			j++;
		}
		if ((args[i][j] == '+' && args[i][j + 1] == '=') || args[i][j] == '=')
			append_var(data, args[i], data->env);
		i++;		
	}
}

void    procede_to_export(t_minishell *data, char **args)
{
    int n_new_var;

    count_new_vars(data, args);
}

void    ft_export_test(t_minishell *data, char **args)
{
    if (args[1] == NULL)
        printf_env_ascii(data);
    else
        procede_to_export(data, args);
}