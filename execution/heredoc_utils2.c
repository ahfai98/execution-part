#include "minishell.h"

void	heredoc_enqueue(t_list **heredoc, char *delimiter, t_token_info *token_info)
{
	char	*buffer;
	int		*fd;

	fd = ft_calloc(2, sizeof(int));
	pipe(fd);
	printf("> ");
	buffer = get_next_line(STDIN_FILENO);
	while (buffer)
	{
		if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter))
			&& (ft_strlen(delimiter) == ft_strlen(buffer) - 1))
		{
			free(buffer);
			break ;
		}
    //EXPAND HERE BEFORE WRITING TO THE TEMPORARY FD!!
    ft_putstr_fd(expand_env(buffer, token_info), fd[1]);
		free(buffer);
		printf("> ");
		buffer = get_next_line(STDIN_FILENO);
	}
	close(fd[1]);
	ft_lstadd_back(heredoc, ft_lstnew(fd));
}

int ms_heredoc_dequeue(t_list **heredoc)
{
	int		res;
	t_list	*temp;

	temp = *heredoc;
	*heredoc = (*heredoc)->next;
	res = *(int *)temp->content;
	ft_lstdelone(temp, &free);
	return (res);
}