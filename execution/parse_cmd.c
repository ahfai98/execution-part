#include "minishell.h"

t_cmd	*parse_cmd_list(t_token *token_list)
{
	t_cmd *cmd_list;
	t_cmd *buffer;

	cmd_list = cmd_list_init(OP_START);
	buffer = cmd_list;
	while (token_list && g_errno == 0)
	{
		if (ft_strncmp(token_list->word, "(", 1) == 0)
			parse_cmd_recurse(token_list, &buffer);
		else if (ft_strncmp(token_list->word, ")", 1) == 0)
			break;
		else if (ft_strncmp(token_list->word, "&&", 2) == 0
			|| ft_strncmp(token_list->word, "||", 2) == 0)
			parse_cmd_next(token_list, &buffer);
		else
		{
			if (parse_pipe_list(token_list, buffer))
				return (cmd_list);
		}
	}
	if ((int)buffer->e_type == -1 && g_errno == 0)
		print_syntax_error(NULL);
	return (cmd_list);
}

void	parse_cmd_recurse(t_token *token_list, t_cmd **buffer)
{
	if ((int)(*buffer)->e_type != -1)
		print_syntax_error(NULL);
	else
	{
		token_list = token_list->next;
		(*buffer)->e_type = CMD_LIST;
		(*buffer)->ptr = parse_cmd_list(token_list);
		token_list = token_list->next;
	}
}

void	parse_cmd_next(t_token *token_list, t_cmd **buffer)
{
	if ((int)(*buffer)->e_type == -1)
		print_syntax_error(NULL);
	else
	{
		if (ft_strncmp(token_list->word, "&&", 2) == 0)
			(*buffer)->next = cmd_list_init(OP_AND);
		else
			(*buffer)->next = cmd_list_init(OP_OR);
		*buffer = (*buffer)->next;
		token_list = token_list->next;
	}
}

t_cmd	*get_cmd_list(t_token_info *token_info)
{
	t_cmd		*cmd_list;

	cmd_list = parse_cmd_list(token_info->token_list);
	return (cmd_list);
}
