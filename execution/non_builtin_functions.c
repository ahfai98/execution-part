#include "minishell.h"

static char	*comb_cmd(char **paths, char *rawcmd)
{
	char	*cmd;
	char	*temp;

	if (access(rawcmd, F_OK) == 0)
		return (rawcmd);
	while (*paths != NULL)
	{
		temp = ft_strjoin(*paths, "/");
		cmd = ft_strjoin(temp, rawcmd);
		if (access(cmd, F_OK) == 0)
		{
			free(temp);
			return (cmd);
		}
		free(cmd);
		free(temp);
		paths++;
	}
	return (rawcmd);
}

int	get_abspath(char **env, char **cmd)
{
	char	**split;
	char	*converted;
	char	*temp;
	char	*path;

	temp = *cmd;
	path = get_env_var("PATH", token_info->env_arr)->value;
	if (path == NULL)
		return (1);
	split = ft_split(path, ':');
	converted = comb_cmd(split, temp);
	free_doublearray(split);
	if (converted != temp)
	{
		free(*cmd);
		*cmd = converted;
	}
	return (0);
}

static void	child_close_fd(t_exe *exec, t_pipe *p)
{
	if (exec->pipe_count != 0)
	{
		close(exec->pipe_fd[exec->pipe_count - 1][0]);
		close(exec->pipe_fd[exec->pipe_count - 1][1]);
	}
	if (p->next)
	{
		close(exec->pipe_fd[exec->pipe_count][0]);
		close(exec->pipe_fd[exec->pipe_count][1]);
	}
}

void	executor_non_bi(t_token_info *token_info, t_exe *exec,
			t_pipe *p, char **argv)
{
	int		pid;
	char	*value;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		child_close_fd(exec, p);
		if (argv[0] != NULL && argv[0][0] != '\0')
			get_abspath(token_info->env_arr, argv);
		execve(argv[0], argv, token_info->env_arr);
		value = get_env_var("PATH", token_info->env_arr)->value;
		ft_putstr_fd("\"", 2)
		ft_putstr_fd(argv[0], 2);
		if (value == NULL)
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}