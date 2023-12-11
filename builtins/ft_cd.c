#include "minishell.h"

int ft_cd(t_token *tokens)
{
    //printf("%s", tokens->content);
    if (tokens != NULL && tokens->content != NULL)
    {
        if (chdir(tokens->content) == 0)
            return 1;
        else
        {
            perror("cd");
            return 0;
        }
    }
    else
    {
        fprintf(stderr, "Invalid token for cd\n");
        return 0;
    }
}
