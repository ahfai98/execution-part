#include "minishell.h"

int	ft_env(t_token_info *token_info, char **args)
{
  t_env *ori_global_env;

  ori_global_env = token_info->global_env;
	while (token_info->global_env != NULL)
	{
    printf("%s=", token_info->global_env->name);
		if (token_info->global_env->value != 0)
			printf("%s", token_info->global_env->value);
    printf("\n");
    token_info->global_env = token_info->global_env->next;
	}
  token_info->global_env = ori_global_env;
	return (0);
	(void)args;
}