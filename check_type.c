#include "minishell.h"

void check_type(t_token **tokens, int arg)
{
    if (strcmp(tokens[arg]->content, ">") == 0)
    {
        tokens[arg]->type = OUTPUT;
    }
    else if (strcmp(tokens[arg]->content, "<") == 0)
    {
        tokens[arg]->type = INPUT;
    }
    else if (strcmp(tokens[arg]->content, ">>") == 0)
    {
        tokens[arg]->type = HEREDOC;
    }
    else if (strcmp(tokens[arg]->content, "<<") == 0)
    {
        tokens[arg]->type = APPEND;
    }
    else
    {
        tokens[arg]->type = CMD;
    }
}
