#include "minishell.h"

int	ft_pwd(t_token_info *token_info, char **args)
{
	char	*path;

	path = getcwd(NULL, 0);
	printf("%s\n", path);
	free(path);
	return (0);
	(void)args;
	(void)token_info;
}