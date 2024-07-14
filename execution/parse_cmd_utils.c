#include "minishell.h"

void  print_syntax_error(t_token *token)
{
  g_errno = 1;
	printf("bash: syntax error near unexpected token `%s'\n",
					token->word);
}

t_cmd	*cmd_list_init(int operator)
{
	t_cmd	*cmd_list;

	cmd_list = ft_calloc(1, sizeof(t_cmd));
	cmd_list->e_operator = operator;
	cmd_list->e_type = -1;
	cmd_list->ptr = NULL;
	cmd_list->next = NULL;
	return (cmd_list);
}

t_pipe	*pipe_list_init(void)
{
	t_pipe	*pipe_list;

	pipe_list = ft_calloc(1, sizeof(t_pipe));
	pipe_list->argv = NULL;
	pipe_list->io_list = NULL;
	pipe_list->next = NULL;
	return (pipe_list);
}