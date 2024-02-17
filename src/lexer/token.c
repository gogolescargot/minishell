/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:53:53 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:53:53 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*new_token(enum e_tokentype type, char *content)
{
	t_token	*head;

	head = malloc(sizeof(t_token));
	if (!head)
		return (NULL);
	head->type = type;
	head->content = content;
	head->prev = NULL;
	head->next = NULL;
	return (head);
}

t_token	*last_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	addback_token(t_token **lst, enum e_tokentype type, char *content)
{
	t_token	*last;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new_token(type, content);
		return ;
	}
	last = last_token(*lst);
	last->next = new_token(type, content);
	last->next->prev = last;
}
