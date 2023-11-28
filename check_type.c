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
        tokens[arg]->type = APPEND;
    }
    else if (strcmp(tokens[arg]->content, "<<") == 0)
    {
        tokens[arg]->type = HEREDOC;
    }
    else
    {
        tokens[arg]->type = CMD;
		if (getcwd(tokens[arg]->content, strlen(tokens[arg]->content)) != NULL)
			getcwd(tokens[arg]->content, strlen(tokens[arg]->content));
		printf("%s	", tokens[arg]->content);
    }
}
