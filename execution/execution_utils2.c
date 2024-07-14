#include "minishell.h"

void	exec_redir_set(t_exe *exec, t_pipe *p)
{
	if (p->next)
		pipe(exec->pipe_fd[exec->pipe_count]);
	if (exec->infile != 0)
		dup2(exec->infile, 0);
	else if (exec->pipe_count != 0)
		dup2(exec->pipe_fd[exec->pipe_count - 1][0], 0);
	if (exec->outfile != 1)
		dup2(exec->outfile, 1);
	else if (p->next)
		dup2(exec->pipe_fd[exec->pipe_count][1], 1);
}

void	exec_redir_reset(t_exe *exec)
{
	if (exec->pipe_count != 0)
	{
		close(exec->pipe_fd[exec->pipe_count - 1][0]);
		close(exec->pipe_fd[exec->pipe_count - 1][1]);
	}
	if (exec->infile != 0)
		close(exec->infile);
	if (exec->outfile != 1)
		close(exec->outfile);
	dup2(exec->tmpstdin, 0);
	dup2(exec->tmpstdout, 1);
}

void	*ft_list_to_array(t_list *lst, int size)
{
	void	*buffer;
	void	*head;

	if (lst == NULL)
		return (NULL);
	buffer = ft_calloc(ft_lstsize(lst), size);
	head = buffer;
	while (lst != NULL)
	{
		ft_memcpy(buffer, lst->content, size);
		lst = lst->next;
		buffer += size;
	}
	return (head);
}
