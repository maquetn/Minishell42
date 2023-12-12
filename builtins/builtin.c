#include "minishell.h"

void exec_builtin(t_token **tokens, int i)
{
    int j = 0;
    while(j <= i)
    {
        if (strcmp(tokens[j]->content, "echo") == 0)
            ft_echo(tokens, j +1);
        else if (strcmp(tokens[j]->content, "cd") == 0)
            ft_cd(tokens[j + 1]);
        else if (strcmp(tokens[j]->content, "pwd") == 0)
            printf("%s\n", ft_pwd());
        else if (strcmp(tokens[j]->content, "ls") == 0)
            ls(ft_pwd());
        else if (strcmp(tokens[j]->content, "export") == 0)
            printf("export function\n"); // replace with ft_export
        else if (strcmp(tokens[j]->content, "unset") == 0)
            printf("unset function\n"); // replace with ft_unset
        else if (strcmp(tokens[j]->content, "env") == 0)
            printf("env function\n"); // replace with ft_env
        else if (strcmp(tokens[j]->content, "exit") == 0)
            ft_exit();
        j++;
    }
}