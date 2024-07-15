#include "minishell.h"

void 	execution(t_token_info *token_info, t_cmd *cmd_list)
{
	t_exe	*exec;

	exec = executor_init();
	heredoc_cmd_queue(exec, cmd_list, 1, token_info);
	executor_cmd_list(token_info, exec, cmd_list);
	executor_free(&exec);
}

void	executor_cmd_list(t_token_info *token_info, t_exe *exec, t_cmd *cmd)
{
	while (cmd)
	{
		if (cmd->e_operator == OP_START
			|| (cmd->e_operator == OP_AND && g_errno == 0)
			|| (cmd->e_operator == OP_OR && g_errno != 0))
		{
			if (cmd->e_type == PIPE_LIST)
				executor_pipe_list(token_info, exec, cmd->ptr);
			else
				executor_cmd_list(token_info, exec, cmd->ptr);
		}
		else
		{
			if (cmd->e_type == PIPE_LIST)
				heredoc_pipe_queue(exec, cmd->ptr, 0, token_info);
			else
				heredoc_cmd_queue(exec, cmd->ptr, 0, token_info);
		}
		cmd = cmd->next;
	}
}

void	executor_pipe_list(t_token_info *token_info, t_exe *exec, t_pipe *pipe)
{
	exec->pipe_count = 0;
	exec->has_child = 0;
	executor_init_pipefd(exec, pipe);
	while (pipe)
	{
		exec->infile = 0;
		exec->outfile = 1;
		executor_io_list(token_info, exec, pipe->io_list);
		if (exec->runtime_error)
			g_errno = exec->runtime_error;
		else
			executor(token_info, exec, pipe);
		pipe = pipe->next;
		exec->pipe_count++;
	}
	executor_free_pipefd(exec);
	if (exec->has_child == 1)
		executor_wait_pipe();
}

void	executor(t_token_info *token_info, t_exe *exec, t_pipe *p)
{
	char	**argv;
	int		bif_n;

	exec_redir_set(exec, p);
	ft_lstadd_back(&p->argv, ft_lstnew(ft_calloc(1, sizeof(char *))));
	argv = ft_list_to_array(p->argv, sizeof(char *));
	bif_n = exec_is_builtin(argv[0]);
	if (argv[0] != NULL && bif_n != -1)
		executor_bi(token_info, argv, bif_n);
	else if (argv[0] != NULL)
	{
		executor_non_bi(token_info, exec, p, argv);
		exec->has_child = 1;
	}
	free(argv);
	exec_redir_reset(exec);
}