#include "minishell.h"

int	get_flag(char **args)
{
	int	i;
	int	j;
	int	flag_count;

	i = 0;
	flag_count = 0;
	while (args[++i] != 0)
	{
		j = 1;
		if (args[i][0] == '-')
			while (args[i][j] == 'n')
				j++;
		if (args[i][j - 1] == 'n' && args[i][j] == '\0')
			flag_count++;
		else
			break ;
	}
	return (flag_count);
}

int	ft_echo(t_token_info *token_info, char **args)
{
	int	i;
	int	flag_count;

	if (args[1] == 0)
		return (printf("\n") - 1);
	flag_count = get_flag(args);
	i = flag_count;
	while (args[++i] != 0)
	{
		printf(args[i]);
		if (args[i + 1] != 0)
			printf(" ");
	}
	if (flag_count == 0)
		ft_printf("\n");
	return (0);
	(void)token_info;
}