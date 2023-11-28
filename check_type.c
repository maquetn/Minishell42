#include <minishell.h>

void check_type(t_token **tokens, int content)
{
    if (strcmp(tokens[content]->content, ">") == 0)
    {
        tokens[content]->type = OUTPUT;
    }
    else if (strcmp(tokens[content]->content, "<") == 0)
    {
        tokens[content]->type = INPUT;
    }
    else if (strcmp(tokens[content]->content, ">>") == 0)
    {
        tokens[content]->type = HEREDOC;
    }
    else if (strcmp(tokens[content]->content, "<<") == 0)
    {
        tokens[content]->type = APPEND;
    }
    else
    {
        tokens[content]->type = CMD;
    }
}
