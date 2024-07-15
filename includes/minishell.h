#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include "libft.h"
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <sys/wait.h>
# include <dirent.h>
# include <stdbool.h>
# include <stdarg.h>
# include <fcntl.h>

# define SHELL_MSG "minishell$ "
# define SPACE_CHAR " \f\v\t\r\n"
# define INVALID_ENV_CHAR "[]<>!@$%%^-+~/ "
# define BASH_CMDS "env,cd,unset,export,exit"
# define BRACE -125
# define INVALID_CHARS "{};*\\"
# define SHELL_OPERATORS "|><()&"

# define T_PIPE 'p'
# define T_CMD 'c'
# define T_REDIR 'r'

# define T_OR 'o'
# define T_AND 'a'
# define T_SUBSHELL '('

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	char			*word;
	char			type;
	char			*infile; //token chunk only
	char			*outfile; //token chunk only
	char			outfile_mode; //token chunk only
	char			**cmds; //token chunk only
	struct s_token	*start; //token chunk only
	char			*heredoc; //token chunk only
	struct s_token	*next; //for BOTH
}	t_token;


typedef struct s_token_info
{
	t_token		*token_list;
	t_token		*quote_list_buffer;
	t_token		*token_chunks;
	int			size;
	char		*str;
	t_token		*cmd_start;
	char		**env_arr;
	bool		has_error;
	t_env		*global_env;
}	t_token_info;

int			g_errno; // Global errno is defined here

typedef struct s_exe
{
	t_list	*heredoc; //heredoc list that contains the piped fds
	int		**pipe_fd; //pipes for fds
	int		pipe_count; //number of pipes
	int		infile; //infile fd
	int		outfile; //outfile fd
	int		tmpstdin; //original stdin used to restore
	int		tmpstdout; //original stdout used to restore
	int		runtime_error; //check for errors during execution
	int		has_child; //check for child processes for non builtin funcs
}	t_exe;

typedef struct s_io
{
	enum
	{
		IO_AIN,
		IO_AOUT,
		IO_IN,
		IO_OUT
	}	e_type; //type of redirection, AIN<<, AOUT>>, IN<, OUT>
	t_list		*value; //filename or delimiter for heredoc
	struct s_io	*next; // pointer to next node in io_list
}	t_io;

typedef struct s_pipe
{
	t_list			*argv; //list of arguments
	t_io			*io_list; //io_list for the pipe
	struct s_pipe	*next; //pointer to next pipe in pipe_list
}	t_pipe;

typedef struct s_cmd
{
	enum
	{
		OP_START,
		OP_AND,
		OP_OR
	}	e_operator; //operator for cmd list when initialised
	enum
	{
		PIPE_LIST,
		CMD_LIST
	}	e_type; 
	//CMD_LIST for grouped commands in subshells
	//PIPE_LIST for simple commands with pipes
	void			*ptr; //ptr to the struct it is holding
	struct s_cmd	*next; //pointer to next node in cmd_list
}	t_cmd;

// utils
int				get_length_of_list(t_token *head);
int				newline(int var);

// debugging
int				print_tokens(t_token_info *token_list, char format);

// free utils
int				free_TokenList(t_token_info *token_list);

// tokenization
t_token			*scan_cmd(t_token_info *token_list);
t_token_info	*process_input(char *str, t_env *env_arr);
t_token			*tokenize(char *string, t_token_info *token_list);
char			**tokens2arr(t_token *token_chunk, t_token *str_end);
t_token			*append_tok(t_token *token, t_token **head);
t_token			*tok(char *word, char type);
int				is_token_valid(char *str, t_token_info *token_info);

// environment variables
t_env			*arr2env(char **env, char **env_arr);
t_env			*get_env_var(char *var_name, t_env **env);
int				print_env(t_env **env_list, char mode);
char			**env2arr(t_env *env);
char			*expand_env(char *string, t_token_info *token_info);
t_env			*new_env(char *name, char *value);
int				unset_env(char *var_name, t_env **envList, 
					t_token_info *token_list);
t_env			*append_env(t_env *env, t_env **envList, char **env_arr);
int				export(t_token_info *token_info);

// quotes
char			toggle_quote_state(char quote, char c);
char			*split_into_quotes(char *str, t_token *tokens, t_token_info *token_info);
int				count_outermost_quotes(char *str);
bool			quote_alternate(char c, char *quote);

//chunking
void			chunk_tokens(t_token_info *token_list);
int				handle_redir(t_token *head, t_token *token_chunk);

// to be removed
void			run_cmds(char **env, t_token_info *token_info);
char			*get_path(char *cmd, char **env);


#endif
