#include "minishell.h"

int	ft_export(t_token_info *token_info, char **args1)
{
	char	**args;
	char	*var_name;
	t_env	*new;

	(void)args1;
	new = token_info->global_env;
	args = token_info->cmd_start->cmds;
	if (!(token_info->token_list->next))
		print_env(&token_info->global_env, 'x');
	else
	{
		if (ft_strchr(token_info->token_list->next->word, '='))
		{
			var_name = ft_substr(token_info->token_list->next->word, 0,
					(size_t)(ft_strchr(token_info->token_list->next->word, '=')
						- token_info->token_list->next->word));
			if (str_charset(var_name, INVALID_ENV_CHAR, CSET_CONTAINS))
				newline(printf("'%s' not valid identifier\n", var_name));
			else
				append_env(new_env(var_name, \
					ft_strdup(ft_strchr(\
						token_info->token_list->next->word, '=') + 1)), \
						&token_info->global_env, token_info->env_arr);
		}
		else if (str_charset(args[1], INVALID_ENV_CHAR, CSET_CONTAINS))
			newline(printf("'%s' not valid identifier\n", args[1]));
	}
	ft_free_array((void **)token_info->env_arr, 0);
	token_info->env_arr = env2arr(new);
	return (0);
}
