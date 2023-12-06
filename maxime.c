#include "minishell.h"

char    *get_env(char *name, char **env)
{
    char    *path;
    int        i;
    int        j;

    i = 0;
    while (env[i])
    {
        j = 0;
        while (env[i][j] && env[i][j] != '=')
            j++;
        path = ft_substr(env[i], 0, j);
        if (strcmp(path, name) == 0)
        {
            free(path);
            return (env[i] + j + 1);
        }
        free(path);
        i++;
    }
    return (NULL);
}

char    *get_path(char *cmd, char **env)
{
    char    *executable;
    int        i;
    char    **paths;
    char    *potential_path;

    i = -1;
    paths = ft_split(get_env("PATH", env), ':');
    if (!paths)
        EXIT_FAILURE;
    while (paths[++i])
    {
        potential_path = ft_strjoin(paths[i], "/");
        executable = ft_strjoin(potential_path, cmd);
        free(potential_path);
        if (access(executable, F_OK | X_OK) == 0)
            {
                free_tabl(paths);
                return(executable);
            }
        free(executable);
    }
    free_tabl(paths);
    return (cmd);
}

void    init_simple_cmd(t_simple_cmd *cmd)
{
    cmd->args = NULL;
    cmd->input = stdin;
    cmd->output = stdout;
}

int    get_cmd(t_token *token, t_minishell *data)
{
    int i;
    char *path;

    if (token->content)
    path = get_path(token->content, data->env);
    create_simple_cmd(data, token, path);
}

t_node    *recursive_parsing(t_token *token, t_minishell *data)
{
    t_node    *prev;
    t_node    *next;

    prev->cmd = get_cmd(token, data);


}

void    planting(t_minishell *data)
{

    recursive_parsing(data->first_token, data);

}

void    create_simple_cmd(t_minishell *data, t_token *token, char *path)
{
    t_simple_cmd    *cmd;
    t_token            *temp_token;
    int                i;

    i = 1;
    temp_token = token;
    while(token)
    {
        if (token->type == PIPE || token->type == APPEND || token->type == HEREDOC)
        {
            //ici aussi je vais devoir faire des trucs
            break ;
        }
        else if (token->type == INPUT)
        {
            cmd->input = token->content;
            break;
        }
        else if (token->type == OUTPUT)
        {
            cmd->output = token->content;
            break;
        }
        else
        {
            i++;
            token = token->next;
        }
    }
    // je dois compter le nombre de str dont j'aurai besoin
}