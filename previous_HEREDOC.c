char    *heredoc_dollar(char *str, t_minishell *data, int j)
{
    char    *translated = NULL;
    char    *placeholder = NULL;
    char    *placeholder2 = NULL;
    int code;
    int i;

    i = 0;
    translated = ft_strdup("", data);
    code = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '$' && str[i + 1] == '$')
        {
            placeholder = ft_strdup("$$", data);
            translated = ft_strjoin(translated, placeholder, data);
            i += 2;
        }
        else if (str[i] == '$' && str[i + 1] == '?')
        {
            code = data->exit_code;
		    placeholder = ft_itoa(code, data);
		    translated = ft_strjoin(translated, placeholder, data);
            i += 2;
        }
        else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
        {
            placeholder2 = ft_strndup(str, i + 1, get_cancer(str, i + 1) - 1, data);
            placeholder = get_env(placeholder2, data->env, data);
            if (placeholder == NULL)
            	placeholder = ft_strdup("", data);
            translated = ft_strjoin(translated, placeholder, data);
            i = get_cancer(str, i + 1);
        }
        else if (str[i] == '$')
        {
            translated = ft_strjoin(translated, "$", data);
            i++;
        }
        else
        {
            translated = ft_strjoin(translated, ft_strndup(str, i, get_dollar(str, i + 1) - 1, data), data);
            i = get_dollar(str, i);
        }
    }
    translated = ft_strjoin(translated, "\n", data);
    return (translated);
}