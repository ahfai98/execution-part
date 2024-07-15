#include "minishell.h"

int	ft_cd(t_token_info *token_info, char **args)
{
	char	*path;

	if (args[1] == 0)
	{
		path = expand_env("$HOME", token_info);
		if (path == NULL)
			ft_putendl_fd("HOME not set", 2);
		else if (chdir(path) < 0)
			perror(path);
		free(path);
	}
	else if (chdir(args[1]) < 0)
		perror(args[1]);
	return (0);
}