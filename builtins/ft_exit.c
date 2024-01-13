#include "../minishell.h"

int ft_exit(t_minishell *data)
{
    printf("\n");
	printf("       \e[1;33m\e[44m ********************************************************* \033[0;37m\n");
	printf("       \e[1;33m\e[44m *                                                       * \033[0;37m\n");
	printf("       \e[1;33m\e[44m *        🐚 🐚 🐚    Exiting Minishell.    🐚 🐚 🐚     * \033[0;37m\n");
	printf("       \e[1;33m\e[44m *                                                       * \033[0;37m\n");
	printf("       \e[1;33m\e[44m ********************************************************* \033[0;37m\n\n");
    free_custom_alloc(data);
    free_tabl(data->env);
    exit(data->exit_code);
}
