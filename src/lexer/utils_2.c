/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:50:33 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:50:33 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_space(char c)
{
	return (ft_strchr(" \t\v", c));
}

bool	is_quote(char c)
{
	return (ft_strchr("\"\'", c));
}

/*
 * Check a quote interpretation and update a quotes state
 *
 * @param c A caracter to check
 * @param quoted A current state of quotes
 */

void	is_quoted(char c, int *quoted)
{
	if (c == '\"' && *quoted == 0)
		*quoted = 1;
	else if (c == '\"' && *quoted == 1)
		*quoted = 0;
	else if (c == '\'' && *quoted == 0)
		*quoted = 2;
	else if (c == '\'' && *quoted == 2)
		*quoted = 0;
}

/*
 * Check if its an operator
 *
 * @param str A string to check
 * @return An enumeration token type of the operator,
 * otherwise enumeration token type TOKEN_NONE
 */

enum e_tokentype	is_operator(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (O_FILE_APPEND);
	else if (str[0] == '<' && str[1] == '<')
		return (HEREDOC);
	else if (str[0] == '>')
		return (O_FILE_TRUNC);
	else if (str[0] == '<')
		return (I_FILE);
	else if (str[0] == '|')
		return (PIPE);
	else if (!is_space(str[0]))
		return (WORD);
	return (TOKEN_NONE);
}
