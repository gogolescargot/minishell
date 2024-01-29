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
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/inc/libft.h"

enum e_tokentype
{
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
}	t_token;

void	lexer(char *str);

#endif