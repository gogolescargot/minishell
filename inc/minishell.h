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
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"

extern int	g_exit_code;

enum e_tokentype
{
	NONE,
	WORD,
	Q_WORD,
	HEREDOC,
	I_FILE,
	O_FILE_TRUNC,
	O_FILE_APPEND,
	PIPE
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

typedef struct s_data
{
	t_list	envp;
}	t_data;

t_token	*lexer(char *str);
bool	is_space(char c);
bool	is_quote(char c);
enum e_tokentype	is_operator(char *str);
void	is_quoted(char c, int *quoted);

void	signal_handler(int code);

t_token	*expander(t_token *lst, t_list *envp);
char	*get_env(char *str, t_list *envp);

bool	is_env(char *s1, char *s2);

int		ft_echo(char **cmd);
int		ft_env(t_list *envp);
int		ft_pwd(void);
int		ft_exit(char **cmd);
int		ft_cd(char **cmd, t_list *envp);
int		ft_export(char **cmd, t_list *envp);
int		ft_unset(char **cmd, t_list *envp);

#endif