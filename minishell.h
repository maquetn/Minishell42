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
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

extern int g_exit_code;

typedef struct s_imple_cmd t_simple_cmd;

typedef enum
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE,
	STR
} t_token_type;

typedef struct s_token
{
  t_token_type type;
  char *content;
  struct s_token *next;
  struct s_token *prev;
} t_token;

typedef struct s_imple_cmd
{
  char			**args; //cmd + args
  char			*path_to_cmd;
  char			*input; //default STDIN else pipeout, fd
  char			*output; //default STDOUT else pipein, fd
  t_simple_cmd	*prev;
  t_simple_cmd	*next;

}	t_simple_cmd;


typedef struct s_minishell
{
	char			**env;
	//int		error;
	t_token 		*first_token;
	t_simple_cmd	*node;
	
} t_minishell;

// typedef struct	s_env
// {
// 	char			*value;
// 	struct s_env	*next;
// }				t_env;

int	ft_strlen(const char *str);
int	ft_strncpy(char *dst, const char *src, int size);

void trim(char *str);
void token(char *input, t_minishell *data);
void expand_path_in_tokens(char **input);

void	restore_terminal(struct termios *original_termios);
void	sig_handler(int signum);

void check_type(t_token **tokens, int arg);


char* expand_path(char *input);

char* get_current_directory();

char *get_path();
int try_executing(char *command, char *path);

char	*ft_substr(char const *s, unsigned int start, unsigned int len);
char	*ft_strjoin(char *s1, char const *s2);
char	**ft_split(char const *s, char c);
t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token);
char	*ft_strdup(const char *s1);
void	planting(t_minishell *data);
void	free_tabl(char **tabl);
void execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data);
void add_token(t_token **head, t_token_type type, char *content, int i);
void	print_nodes(t_minishell *data);


// BUILTINS

void exec_builtin(t_simple_cmd *cmd);
void ft_echo(char **args, int index);
int ft_exit(int i, char **args);
int ft_cd(char *token);
char* ft_pwd();
void	ft_env();

void execute_command(t_simple_cmd *cmd, t_minishell *data);


typedef struct s_env_var
{
    char *name;
    char *value;
} EnvVar;

typedef struct s_environment
{
    EnvVar *vars;
    size_t size;
} Environment;

int ft_export(Environment *env, const char *name, const char *value);


char *expand_env_variables(char *str, char **env);

#endif
