#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>


typedef enum
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE,
	CMD
} t_token_type;

typedef struct s_token
{
  t_token_type type;
  char *content;
  //struct s_token *next;
  //struct s_token *prev;
} t_token;


// typedef struct s_imple_cmd
// {
//   char **args; //cmd + args
//   int input; //default STDIN else pipeout, fd
//   int output; //default STDOUT else pipein, fd
//		char 	*path
//		char	*cmd
//   struct s_imple_cmd *next;
//   struct s_imple_cmd *prev;
// //etc
// }

typedef struct s_minishell
{
	char	**env;
	//int		error;
	//s_token *first_token;
	
} t_minishell;

int	ft_strlen(const char *str);
int	ft_strncpy(char *dst, const char *src, int size);

void trim(char *str);
void token(char *input);
void expand_path_in_tokens(char **input);

void	restore_terminal(struct termios *original_termios);
void	sig_handler(int signum);

void check_type(t_token **tokens, int arg);


char* expand_path(char *input);

char* get_current_directory();

void exec_builtin(t_token **tokens, int i);


char *get_path();
int try_executing(char *command, char *path);

int ft_exit();



void ft_echo(t_token **tokens, int index);
char* ft_pwd();
int ft_cd(t_token *tokens);

#endif
