#include "minishell.h"

int	ft_exit(t_token_info *token_info, char **args)
{
	printf("exit\n");
	exit(EXIT_SUCCESS);
	return (0);
	(void)token_info;
	(void)args;
}