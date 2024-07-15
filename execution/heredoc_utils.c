#include "minishell.h"

void	heredoc_cmd_queue(t_exe *exec, t_cmd *cmd_list,
			int state, t_token_info *token_info)
{
	while (cmd_list)
	{
		if (cmd_list->e_type == PIPE_LIST)
			heredoc_pipe_queue(exec, cmd_list->ptr, state, token_info);
		else
			heredoc_cmd_queue(exec, cmd_list->ptr, state, token_info);
		cmd_list = cmd_list->next;
	}
}

void	heredoc_pipe_queue(t_exe *exec, t_pipe *pipe_list, int state,
			t_token_info *token_info)
{
	while (pipe_list)
	{
		if (state == 1)
			heredoc_io_list_enqueue(exec, pipe_list->io_list, token_info);
		else
			heredoc_io_list_dequeue(exec, pipe_list->io_list);
		pipe_list = pipe_list->next;
	}
}

void	heredoc_io_list_enqueue(t_exe *exec, t_io *io_list, t_token_info *token_info)
{
	while (io_list)
	{
		if (io_list->e_type == IO_AIN)
			heredoc_enqueue(&exec->heredoc,
				*(char **)io_list->value->content, token_info);
		io_list = io_list->next;
	}
}

void	heredoc_io_list_dequeue(t_exe *exec, t_io *io_list)
{
	int	fd;

	while (io_list)
	{
		if (io_list->e_type == IO_AIN)
		{
			fd = heredoc_dequeue(&exec->heredoc);
			close(fd);
		}	
		io_list = io_list->next;
	}
}