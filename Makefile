NAME = minishell

SRCS = token.c \
	   ast.c \
	   utils.c \
       main.c \
	   exec.c \
	   builtins/ft_cd.c \
	   builtins/ft_echo.c \
	   builtins/ft_exit.c \
	   builtins/ft_env.c \
	   builtins/ft_pwd.c \
	   builtins/ft_export.c \
	   builtins/ft_unset.c \
	   expander.c \
	   custom_malloc.c \
	   heredoc.c

OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I /Users/$(USER)/.brew/opt/readline/include #-fsanitize=address -g

LDFLAGS = -lreadline -lhistory -L /Users/$(USER)/.brew/opt/readline/lib

RM = rm -rf

# Extracting subdirectories from SRCS
SUBDIRS = $(sort $(dir $(SRCS)))

# Create the directory structure in OBJ_DIR
OBJ_SUBDIRS = $(addprefix $(OBJ_DIR)/, $(SUBDIRS))

all: $(OBJ_SUBDIRS) $(NAME)

$(OBJ_SUBDIRS):
	@mkdir -p $@

${OBJ_DIR}/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
