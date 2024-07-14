#include "minishell.h"

void  free_doublearray(char **split)
{
  int i;

  i = 0;
  while (split[i] != NULL)
  {
    free(split[i]);
    i++;
  }
  free (split);
}

int	exec_is_builtin(char *command)
{
	char	**bif;
	int		i;

	bif = ft_split("echo cd pwd export unset env exit", ' ');
	i = 0;
	while (bif[i] != NULL)
	{
		if (ft_strcmp(command, bif[i]) == 0)
    {
      free_doublearray(bif);
			return (i);
    }
    i++;
	}
	free_doublearray(bif);
	return (-1);
}

void	executor_bi(t_token_info *token_info, char **command, int bif_n)
{
  if (bif_n == 0)
	  g_errno = ft_echo(token_info, command);
  else if (bif_n == 1)
	  g_errno = ft_cd(token_info, command);
  else if (bif_n == 2)
	  g_errno = ft_pwd(token_info, command);
  else if (bif_n == 3)
	  g_errno = ft_export(token_info, command);
  else if (bif_n == 4)
	  g_errno = ft_unset(token_info, command);
  else if (bif_n == 5)
	  g_errno = ft_env(token_info, command);
  else if (bif_n == 1)
	  g_errno = ft_exit(token_info, command);
}