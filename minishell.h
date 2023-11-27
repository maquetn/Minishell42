/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaquet <nmaquet@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:21:16 by abuchet           #+#    #+#             */
/*   Updated: 2023/11/27 16:40:15 by nmaquet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	g_status = 0;

typedef enum
{
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
	PIPE,
	CMD
} t_oken_type;

typedef struct s_token
{
  t_oken_type type;
  char  *content;
  struct s_token *next;
  struct s_token *prev;
} t_token;

// typedef struct s_imple_cmd
// {
//   char **args //cmd + args
//   int input; //default STDIN else pipeout, fd
//   int output; //default STDOUT else pipein, fd
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
int token(char *input);


void	restore_terminal(struct termios *original_termios);
void sig_handler(int signum);




#endif