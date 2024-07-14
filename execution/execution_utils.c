#include "minishell.h"

t_exe	*executor_init(void)
{
	t_exe	*exec;

	exec = ft_calloc(1, sizeof(t_exe));
	exec->heredoc = NULL;
	exec->pipe_fd = NULL;
	exec->infile = 0;
	exec->outfile = 1;
	exec->tmpstdin = dup(0);
	exec->tmpstdout = dup(1);
	exec->runtime_error = 0;
	return (exec);
}

void	executor_free(t_exe **exec)
{
	close((*exec)->tmpstdin);
	close((*exec)->tmpstdout);
	free(*exec);
	*exec = NULL;
}

void	executor_init_pipefd(t_exe *exec, t_pipe *pipe)
{
	int	i;

	i = 0;
	while (pipe)
	{
		pipe = pipe->next;
		i++;
	}
	exec->pipe_fd = ft_calloc(i + 1, sizeof(int *));
	exec->pipe_fd[i] = NULL;
	while (i--)
		exec->pipe_fd[i] = ft_calloc(2, sizeof(int));
}

void	executor_wait_pipe(void)
{
	int	returnpid;
	int	status;

	returnpid = 1;
	while (returnpid > 0)
		returnpid = waitpid(-1, &status, WUNTRACED);
	if (WIFEXITED(status))
		g_errno = (WEXITSTATUS(status));
}

void	executor_free_pipefd(t_exe *exec)
{
	int	i;

	i = 0;
	while (exec->pipe_fd[i])
	{
		free(exec->pipe_fd[i]);
		i++;
	}
	free(exec->pipe_fd);
	exec->pipe_fd = NULL;
}