/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 19:27:57 by ggalon            #+#    #+#             */
/*   Updated: 2024/01/27 19:27:57 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_token	*new_token(enum e_tokentype type, char *content)
{
	t_token	*head;

	head = malloc(sizeof(t_token));
	if (!head)
		return (NULL);
	head->type = type;
	head->content = content;
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
}

char	*word(char *str, size_t *spc)
{

}

void	print_token(t_token *lst)
{
	while (lst)
	{
		if (lst->type == WORD || lst->type == Q_WORD)
			printf("%s\n", lst->content);
		if (lst->type == I_FILE)
			printf("Input :%s\n", lst->content);
		if (lst->type == O_FILE_APPEND)
			printf("Append :%s\n", lst->content);
		if (lst->type == O_FILE_TRUNC)
			printf("Trunc :%s\n", lst->content);
		if (lst->type == HEREDOC)
			printf("Heredoc :%s\n", lst->content);
		if (lst->type == PIPE)
			printf("%s\n", lst->content);
		lst = lst->next;
	}
}

int	is_operator(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (1);
	else if (str[0] == '<' && str[1] == '<')
		return (2);
	else if (str[0] == '>')
		return (3);
	else if (str[0] == '<')
		return (4);
	else if (str[0] == '|')
		return (5);
	return (0);
}

void	lexer(char *str)
{
	size_t	j;
	t_token	*lst;

	j = 0;
	lst = NULL;
	while (str[j])
	{

	}
	print_token(lst);
}
