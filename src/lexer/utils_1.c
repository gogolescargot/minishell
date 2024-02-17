/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:51:16 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:51:16 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Check if the word is well quoted and not NULL
 *
 * @param str A string to check
 * @return True if the synthax is correct, otherwise False
 */

bool	check_word(char *str)
{
	int		quoted;
	size_t	i;

	quoted = 0;
	i = 0;
	if (!str)
		return (false);
	while (str[i])
	{
		is_quoted(str[i], &quoted);
		i++;
	}
	return (quoted == 0);
}

bool	check_pipe_prev(t_token *lst)
{
	while (lst)
	{
		if (lst->type == WORD)
			return (true);
		lst = lst->prev;
	}
	return (false);
}

bool	check_pipe_next(t_token *lst)
{
	while (lst)
	{
		if (lst->type == WORD)
			return (true);
		lst = lst->next;
	}
	return (false);
}

/*
 * Check if there is any WORD token before and after the PIPE token
 *
 * @param lst A linked list that contain all the tokens to check
 * @return True if the synthax is correct, otherwise False
 */

bool	check_pipe(t_token *lst)
{
	return (check_pipe_prev(lst) && check_pipe_next(lst));
}

/*
 * Check all tokens synthax
 *
 * @param lst A linked list that contain all the tokens to check
 * @return 0 if the synthax is correct, otherwise non-zero value
 */

int	check_token(t_token *lst)
{
	if (!lst)
		return (-1);
	while (lst)
	{
		if (lst->type == PIPE && !check_pipe(lst))
			return (1);
		else if (lst->type != NONE
			&& lst->type != PIPE && !check_word(lst->content))
			return (2);
		else if (lst->type == NONE)
			return (3);
		lst = lst->next;
	}
	return (0);
}
