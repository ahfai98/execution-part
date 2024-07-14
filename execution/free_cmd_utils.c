#include "minishell.h"

void	cmd_list_free(t_cmd **cmd_list)
{
	t_cmd	*temp;

	while (*cmd_list)
	{
		temp = *cmd_list;
		*cmd_list = (*cmd_list)->next;
		if (temp->e_type == PIPE_LIST)
			pipe_list_free((t_pipe **)&temp->ptr);
		else
			cmd_list_free((t_cmd **)&temp->ptr);
		free(temp);
	}
}

void	pipe_list_free(t_pipe **pipe_list)
{
	t_pipe	*temp;

	while (*pipe_list)
	{
		temp = *pipe_list;
		*pipe_list = (*pipe_list)->next;
		ft_lstclear(&temp->argv, &free_args);
		io_list_free(&temp->io_list);
		free(temp);
	}
}

void	free_args(void *content)
{
	if (*(char **)content)
		free(*(char **)content);
	free(content);
}

void	io_list_free(t_io **io_list)
{
	t_io	*temp;

	while (*io_list)
	{
		temp = *io_list;
		*io_list = (*io_list)->next;
		ft_lstclear(&temp->value, &free_args);
		free(temp);
	}
}