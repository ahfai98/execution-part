#include "minishell.h"

int	parse_pipe_list(t_token *token_list, t_cmd *buffer)
{
	if ((int)buffer->e_type != -1)
	{
		print_syntax_error(NULL);
		return (1);
	}
	buffer->e_type = PIPE_LIST;
	buffer->ptr = parse_pipe_list_sub(token_list);
	return (g_errno);
}

t_pipe	*parse_pipe_list_sub(t_token *token_list)
{
	t_pipe	*pipe_list;
	t_pipe	*buffer;

	pipe_list = pipe_list_init();
	buffer = pipe_list;
	while (token_list && g_errno == 0)
	{
		if (ft_strncmp(token_list->word, "|", 1) == 0
			&& parse_pipe_next(&buffer))
			return (pipe_list);
		else if (is_io_token(token_list->word)
			&& parse_io_list(token_list, &buffer->io_list))
			return (pipe_list);
		else if (check_valid_filename(token_list) == 1)
			pipe_new_arg(token_list, buffer);
		else
			break;
		token_list = token_list->next;
	}
	if (buffer->argv == NULL && buffer->io_list == NULL)
		print_syntax_error(NULL);
	return (pipe_list);
}

int	parse_pipe_next(t_pipe **buffer)
{
	if ((*buffer)->argv == NULL && (*buffer)->io_list == NULL)
	{
		print_syntax_error(NULL);
		return (1);
	}
	(*buffer)->next = pipe_list_init();
	*buffer = (*buffer)->next;
	return (0);
}

void	pipe_new_arg(t_token *token_list, t_pipe *buffer)
{
	t_list	*new;

	new = ft_lstnew(ft_calloc(1, sizeof(char *)));
	ft_memcpy(new->content, token_list->word, sizeof(char *));
	ft_lstadd_back(&buffer->argv, new);
}

int	parse_io_list(t_token *token_list, t_io **io_list)
{
	t_io	*new;
	t_io	*buffer;
	int		type;

	type = check_io_type(token_list);
	token_list = token_list->next;
	if (check_valid_filename(token_list) == 0)
	{
		print_syntax_error(token_list);
		return (1);
	}
	new = io_list_init(type);
	new->value = ft_lstnew(ft_calloc(1, sizeof(char *)));
	ft_memcpy(new->value->content, token_list->word, sizeof(char *));
	if (*io_list != NULL)
	{
		buffer = *io_list;
		while (buffer->next != NULL)
			buffer = buffer->next;
		buffer->next = new;
	}
	else
		*io_list = new;
	return (0);
}
