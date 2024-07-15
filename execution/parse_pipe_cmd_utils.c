#include "minishell.h"

int	is_io_token(t_token *token)
{
	return (ft_strncmp(token->word, "<<", 2) == 0
		|| ft_strncmp(token->word, "<", 1) == 0
		|| ft_strncmp(token->word, ">>", 2) == 0
		|| ft_strncmp(token->word, ">", 1) == 0);
}

int check_io_type(t_token *token)
{
	if (ft_strncmp(token->word, "<<", 2) == 0)
		return (IO_AIN);
	else if (ft_strncmp(token->word, "<", 1) == 0)
		return (IO_IN);
	else if (ft_strncmp(token->word, ">>", 2) == 0)
		return (IO_AOUT);
	return (IO_OUT);
}

t_io	*io_list_init(int type)
{
	t_io	*io_list;

	io_list = ft_calloc(1, sizeof(t_io));
	io_list->e_type = type;
	io_list->value = NULL;
	io_list->next = NULL;
	return (io_list);
}

int check_valid_filename(t_token *token)
{
	int i;
	const char *invalid_chars = "\\/:*?\"<>|";

	i = 0;
	if (token == NULL || token->word == NULL || token_word[0] == '\0')
		return (0);
	if (ft_isdigit(token->word[0]))
			return (0);
	if (ft_strlen(token->word) > 255)
		return (0);
	while(token->word[i])
	{
		if (is_alnum(token->word[i]) == 0 && token_word[i] != '_')
			return (0);
		if (ft_strchr(invalid_chars, token->word[i]))
			return (0);
		i++;
	}
	return (1);
}