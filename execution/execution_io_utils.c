#include "minishell.h"

int	executor_ambiguous(t_exe *exec, t_list *value, char *env_val)
{
	if (ft_lstsize(value) != 1)
	{
		ft_putstr_fd("minishell: ambiguous redirect", 2);
		ft_putendl_fd(env_val, 2);
		exec->runtime_error = 1;
		return (1);
	}
	return (0);
}

void	executor_io_in(t_exe *exec, t_io *io)
{
	if (exec->infile != STDIN_FILENO)
		close(exec->infile);
	if (io->e_type == IO_AIN)
		exec->infile = heredoc_dequeue(&exec->heredoc);
	else
		exec->infile = open(*(char **)io->value->content, O_RDONLY);
}

void	executor_io_out(t_exe *exec, t_io *io)
{
	if (exec->outfile != STDOUT_FILENO)
		close(exec->outfile);
	if (io->e_type == IO_AOUT)
		exec->outfile = open(*(char **)io->value->content,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		exec->outfile = open(*(char **)io->value->content,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
}

int	executor_check_file_error(t_exe *exec, char *filename)
{
	if (exec->infile == -1 || exec->outfile == -1)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(filename, 2);
		ft_putchar_fd(':', 2);
		ft_putendl_fd(strerror(errno), 2);
		exec->runtime_error = 1;
	}
	return (exec->runtime_error);
}

void	executor_io_list(t_token_info *token_info, t_exe *exec, t_io *io)
{
	char	*env_val;

	while (io)
	{
		if (io->e_type != IO_AIN)
		{
			env_val = ft_strdup(*(char **)io->value->content);
			if (executor_ambiguous(exec, io->value, env_val))
			{
				free(env_val);
				return ;
			}
			free(env_val);
		}
		if (io->e_type == IO_AIN || io->e_type == IO_IN)
			executor_io_in(exec, io);
		else if (io->e_type == IO_AOUT || io->e_type == IO_OUT)
			executor_io_out(exec, io);
		if (executor_check_file_error(exec, *(char **)io->value->content))
			return ;
		io = io->next;
	}
}