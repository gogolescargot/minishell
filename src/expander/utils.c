/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 06:05:36 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 06:05:36 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Check if we are between quotes
 *
 * @param c A character to check
 * @param quoted A current state of quotes
 * @return A new state of quotes
 */

bool	is_interpreted_quote(char c, int quoted)
{
	return ((c == '\'' && (quoted == 2 || quoted == 0))
		|| (c == '\"' && (quoted == 1 || quoted == 0)));
}

/*
 * Calculate the size of the new content
 *
 * @param str A string to handle
 * @param envp A linked list that contain the environement variables
 * @return A calculated new lengh
 */

size_t	content_len(char *str, t_list *envp)
{
	size_t	i;
	size_t	len;
	int		quoted;

	i = 0;
	len = 0;
	quoted = 0;
	while (str[i])
	{
		is_quoted(str[i], &quoted);
		if (str[i] == '$' && quoted < 2)
		{
			len += ft_strlen(get_env(str + i + 1, envp));
			i += getenv_skip(str + i + 1) + 1;
		}
		else if (is_interpreted_quote(str[i], quoted))
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
