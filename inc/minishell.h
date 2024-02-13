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

t_token	*lexer(char *str);
bool	is_space(char c);
bool	is_quote(char c);
void	is_quoted(char c, int *quoted);

t_token	*expander(t_token *lst);

#endif