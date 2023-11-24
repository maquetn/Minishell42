NAME = minishell

SRCS = ft_split.c \
       ft_strncpy.c \
       token.c \
       main.c

OBJ_DIR = obj
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

READLINE_PATH := /Users/$(USER)/homebrew/opt/readline

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(READLINE_PATH)/include

LDFLAGS = -lreadline -dL$(READLINE_PATH)/lib

RM = rm -rf

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

${OBJ_DIR}/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
