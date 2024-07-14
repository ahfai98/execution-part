#include "minishell.h"


int	check_valid_identifier(char *arg, char *str, char *type)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) == 0 && str[i] != '_')
  {
    ft_putstr_fd("unset: '", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
    return (0);
  }
	while (str[++i] != '\0')
  {
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
    {
      ft_putstr_fd("unset: '", 2);
      ft_putstr_fd(arg, 2);
      ft_putstr_fd("': not a valid identifier\n", 2);
      return (0);
    }
  }
	return (1);
}

void	find_and_delete(t_token_info *token_info, char *key)
{
  t_env *new_global_env;
  t_env *env_to_del;
  t_env *prev;
  t_env *curr;

  if (ft_strcmp(token_info->global_env->name, key) == 0)
  {
    env_to_del = token_info->global_env;
    new_global_env = token_info->global_env->next;
  }
  else
  {
    new_global_env = token_info->global_env;
    prev = token_info->global_env;
    curr = token_info->global_env->next;
    while (curr != NULL)
    {
      if (strcmp(curr->name, key) == 0)
      {
        prev->next = curr->next;
        env_to_del = curr;
        break;
      }
      prev = curr;
      curr = curr->next;
    }
  }
  //free(env_to_del); free everything for this t_env *
  token_info->global_env = new_global_env;
}

int	ft_unset(t_token_info *token_info, char **args)
{
	int		i;

	i = 0;
	while (args[++i] != 0)
	{
		if (check_valid_identifier(args[i], args[i], "unset") == 1)
			find_and_delete(token_info, args[i]);
		else
			return (1);
	}
	return (0);
}