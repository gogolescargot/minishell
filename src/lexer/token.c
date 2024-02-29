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

t_token	*token_new(enum e_tokentype type, char *content)
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

t_token	*token_last(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	token_addback(t_token **lst, t_token *token)
{
	t_token	*last;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = token;
		return ;
	}
	last = token_last(*lst);
	last->next = token;
	last->next->prev = last;
}

void	token_clear(t_token **lst, void (*del)(void *))
{
	t_token	*temp;

	if (!(*lst) || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		del((*lst)->content);
		ft_free(*lst);
		*lst = NULL;
		(*lst) = temp;
	}
}
