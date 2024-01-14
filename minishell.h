/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdor <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 19:24:58 by mdor              #+#    #+#             */
/*   Updated: 2024/01/12 19:25:00 by mdor             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <string.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <stdbool.h>

typedef	struct	s_imple_cmd	t_simple_cmd;
typedef struct	s_malloc t_malloc;
typedef struct	s_files_list t_files_list;

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
	t_token_type	type;
	char			*content;
	struct s_token	*next;
	struct s_token	*prev;
	int				quoted_heredoc;
} t_token;

typedef struct s_files_list
{
	char			*name;
	t_files_list 	*next;
} t_files_list;

typedef struct s_imple_cmd
{
	char			**args;
	char			*path_to_cmd;
	t_files_list	*input;
	t_files_list	*output;
	char			*heredoc_string;
	int				append_mode;
	int				heredoc;
	t_simple_cmd	*prev;
	t_simple_cmd	*next;

}	t_simple_cmd;

typedef struct s_malloc
{
	void		*adress;
	t_malloc	*next;
} t_malloc;

typedef struct s_minishell
{
	char			**env;
	t_token			*first_token;
	t_simple_cmd	*node;
	t_malloc		*head;
	int				exit_code;
	int				error_trigger;
	
} t_minishell;

int		ft_strlen(const char *str);
void	token(char *input, t_minishell *data);
void	restore_terminal(struct termios *original_termios);
void	sig_handler(int signum);
void	expand_heredoc(t_token *token, t_minishell *data);
int		need_refine(char *str);
int		quoted(char *str);
void	rewind_tokens(t_minishell *data);
int		create_all_open_last(t_simple_cmd *cmd);
char	*heredoc_delim(char *str, t_minishell *data);
int		ft_length(long n);
char	*ft_do(char *new, int len, long nbr, int neg);
void	handle_builtin(t_simple_cmd *cmd, t_minishell *data, int *pp_fd);
void	child(t_simple_cmd *cmd, t_minishell *data, int *pipe_fd, int *pp_fd);
void	parent(t_simple_cmd *cmd, t_minishell *data, int *pipe_fd, pid_t child_pid);
void	redirect_output(t_simple_cmd *cmd, int *p_fd);
void	redirect_input(t_simple_cmd *cmd, int *p_fd, t_minishell *data);
void	execute_command(t_simple_cmd *cmd, t_minishell *data);
int		open_all(t_simple_cmd *cmd);
int		get_dollar(char *str, int i);
int		redirect_heredoc(t_simple_cmd *cmd, t_minishell *data);
char	*process_double_dollar(char *translated, int *i, t_minishell *data);
char	*process_exit_code(char *translated, int *i, t_minishell *data);
char	*process_env_variable(char *str, char *translated, int *i, t_minishell *data);
char	*process_single_dollar(char *translated, int *i, t_minishell *data);
char	*process_other_chars(char *str, char *translated, int *i, t_minishell *data);
void	process_pipe_token(t_minishell *data, t_token **token);
void	process_heredoc(t_minishell *data, t_token **token, t_simple_cmd *cmd);
int		process_append(t_minishell *data, t_token **token, t_simple_cmd *cmd);
int		process_input(t_minishell *data, t_token **token, t_simple_cmd *cmd);
int		process_output(t_minishell *data, t_token **token, t_simple_cmd *cmd);
t_simple_cmd	*get_cmd(t_token *token, t_minishell *data);
t_simple_cmd	*recursive_parsing(t_minishell *data);
char	*ft_substr(char const *s, int start, int len, t_minishell *data);
char	*ft_strjoin(char *s1, char const *s2, t_minishell *data);
char	**ft_split(char const *s, char c, t_minishell *data);
t_simple_cmd	*create_simple_cmd(t_minishell *data, t_token *token);
char	*ft_strdup(const char *s1, t_minishell *data);
void	planting(t_minishell *data);
void	free_tabl(char **tabl);
void	execute_simple_cmd(t_simple_cmd *cmd, t_minishell *data, int *prev_pipe_fd);
void	add_token(t_token **head, t_token_type type, char *content, t_minishell *data);
void	print_nodes(t_minishell *data);
int		_next(char *str, int i);
char	*ft_itoa(int n, t_minishell *data);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
char	*get_env(char *name, char **env, t_minishell *data);
char	*get_path(char *cmd, char **env, t_minishell *data);
int		add_file(char *name, t_minishell *data, t_files_list **head);
void	init_simple_cmd(t_simple_cmd *cmd);
char	*ft_strndup(char *str, int start, int end, t_minishell *data);
void	expander(t_minishell *data);
void	print_tokens(t_token *head);
int		remove_double(char *str, int i, char **expanded, t_minishell *data);
int		remove_single(char *str, int i, char **expanded, t_minishell *data);
void	*gc_malloc(size_t required_memory ,t_minishell *data);
void	free_custom_alloc(t_minishell *data);
int		ft_strcmp(char *s1, char *s2, t_minishell *data);
int		get_cancer(char *str, int i);
void	ft_putstr_fd(char *s, int fd);
char	*manage_heredoc(char *delim, t_minishell *data, int quoted);
char	*heredoc_dollar(char *str, t_minishell *data, int j);
int		print_syntax_error(t_token *token);
int		dollar(char *str, int i, char **expanded, t_minishell *data, int quoted);
int		calculate_translated_length(const char *str);
char	*build_translated_string(char *str, int len, t_minishell *data);
bool	is_quote_char(char c);
bool	is_escape_sequence(const char *str, int index);
char	**copy_env(char **env);
char    *no_gc_strndup(char *str, int start, int end);
char	*no_gc_itoa(int n);
char	*no_gc_strjoin(char *s1, char const *s2);
char	*no_gc_strdup(const char *s1);

// BUILTINS

void	ft_echo(char **args, int index);
int		ft_exit(t_minishell *data);
int		ft_cd(t_minishell *data, char *token);
char*	ft_pwd();
void	ft_export(t_minishell *data, char **args);
void	ft_unset(t_minishell *data, char **args);
void	ft_env(t_minishell *data);

#endif
