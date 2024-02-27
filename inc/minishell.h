/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:12:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/01/27 18:12:07 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h> 
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"

extern int	g_exit_code;

enum e_tokentype
{
	TOKEN_NONE,
	WORD,
	Q_WORD,
	HEREDOC,
	I_FILE,
	O_FILE_TRUNC,
	O_FILE_APPEND,
	PIPE
};

enum e_builtin
{
	BUILTIN_NONE,
	ECHO,
	ENV,
	PWD,
	EXIT,
	CD,
	EXPORT,
	UNSET,
};

typedef struct s_token
{
	enum e_tokentype	type;
	char				*content;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

typedef struct s_expand
{
	char	*new;
	char	*old;
	size_t	i;
	size_t	j;
	int		quoted;
}	t_expand;

typedef struct s_redir
{
	int		file_fdout;
	int		fdin;
	int		fdout;
	int		tmp_fdin;
	int		tmp_fdout;
}	t_redir;

typedef struct s_data
{
	t_token	*tokens;
	t_list	*envp_lst;
	char	***cmd;
	char	**envp;
	char	*line;
}	t_data;

t_token				*lexer(char *str);
void				expander(t_data *data);
void				execution(t_data *data);

bool				is_space(char c);
bool				is_quote(char c);
bool				is_env(char *s1, char *s2);
bool				is_interpreted_quote(char c, int quoted);
void				is_quoted(char c, int *quoted);
enum e_tokentype	is_operator(char *str);
enum e_builtin		is_builtin(char *str);

char				*get_env(char *str, t_list *envp);
int					update_env(t_list *envp, char *key, char *value);
size_t				getenv_skip(char *str);

int					check_token(t_token *lst);
size_t				content_len(char *str, t_list *envp);

void				token_addback(t_token **lst,
						enum e_tokentype type, char *content);
void				token_clear(t_token **lst, void (*del)(void *));

int					ft_echo(char **cmd);
int					ft_env(t_list *envp);
int					ft_pwd(char **cmd);
int					ft_exit(char **cmd, t_data *data);
int					ft_cd(char **cmd, t_list *envp);
int					ft_export(char **cmd, t_list *envp);
int					ft_unset(char **cmd, t_list *envp);

void				signal_handler(int code);

size_t				get_cmd_nbr(t_token *tokens);
size_t				get_cmd_size(t_token *tokens);
char				*get_cmd_path(char *cmd, t_list *envp_lst);
char				**env_lst_to_str(t_list *envp_lst);

int					commands_fill(t_data data);
void				commands_execute(t_data *data);
void				commands_clear(char ****cmd);

void				ft_close(int fd);
void				ft_close_unlink(int fd, char *file);
void				close_fds_redir(t_redir redir);
int					handle_error(char *str, int error_code);

void				wait_process(int pid);
void				exec_bin(char **cmd, t_data *data,
						t_redir redir, pid_t *pid);
void				exec_builtin(char **args, t_data *data,
						t_redir redir, pid_t *pid);

int					redir_init(t_redir *redir, t_data *data);
void				redir_end(t_redir redir, int pid);
int					redir_in(t_redir *redir, t_token *tokens);
int					redir_pipe(t_redir *redir);
int					redir_out(t_redir *redir, t_token *tokens);

int					heredoc(char *limiter);
void				secure_exit(t_data **data, int error_code);

#endif