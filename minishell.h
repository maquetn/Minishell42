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

typedef struct s_imple_cmd t_simple_cmd;
typedef struct s_malloc t_malloc;
typedef struct s_files_list t_files_list;

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
  int   simple_quotes;
} t_token;

typedef struct s_files_list
{
  char  *name;
  t_files_list  *next;
} t_files_list;

typedef struct s_imple_cmd
{
  char			**args; //cmd + args
  char			*path_to_cmd;
  char			*input; //default STDIN else pipeout, fd
  t_files_list			*output; //default STDOUT else pipein, fd
  int			  append_mode;
  int       heredoc;
  t_simple_cmd	*prev;
  t_simple_cmd	*next;

}	t_simple_cmd;

typedef struct s_malloc
{
  void  *adress;
  t_malloc  *next;
} t_malloc;

typedef struct s_minishell
{
	char			**env;
	//int		error;
	t_token 		*first_token;
	t_simple_cmd	*node;
  t_malloc    *head;
	int				exit_code;
	
} t_minishell;

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

char	*ft_substr(char const *s, unsigned int start, unsigned int len, t_minishell *data);
char	*ft_strjoin(char *s1, char const *s2, t_minishell *data);
char	**ft_split(char const *s, char c, t_minishell *data);
t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token);
char	*ft_strdup(const char *s1, t_minishell *data);
void	planting(t_minishell *data);
void	free_tabl(char **tabl);
void execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data, int *prev_pipe_fd);
void add_token(t_token **head, t_token_type type, char *content, t_minishell *data);
void	print_nodes(t_minishell *data);
char	*ft_itoa(int n, t_minishell *data);
int	ft_isalpha(int c);
int	ft_isalnum(int c);
char	*get_env(char *name, char **env, t_minishell *data);
char    *ft_strndup(char *str, int start, int end, t_minishell *data);
void	expander(t_minishell *data);
void    print_tokens(t_token *head);
int remove_double(char *str, int i, char **expanded, t_minishell *data);
void    *gc_malloc(size_t required_memory ,t_minishell *data);
void    free_custom_alloc(t_minishell *data);
int	ft_strcmp(char *s1, char *s2);
int get_cancer(char *str, int i);
void	ft_putstr_fd(char *s, int fd);
void    translate_heredoc(t_simple_cmd *cmd, t_minishell *data);
char    *manage_heredoc(char *delim, t_minishell *data);
char    *heredoc_dollar(char *str, t_minishell *data);

// BUILTINS

void exec_builtin(t_simple_cmd *cmd);
void ft_echo(char **args, int index);
int ft_exit();
int ft_cd(char *token);
void ft_ls(const char *directory_path);
char* ft_pwd();
void ft_export(t_minishell *data, char **args);
void ft_unset(t_minishell *data, char **args);
void ft_env(t_minishell *data);

#endif