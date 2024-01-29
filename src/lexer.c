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

char	*word(char *str, size_t *j)
{
	while (str[*j] && str[*j] != ' ')
		(*j)++;
	return (ft_substr(str, 0, *j));
}

char	*quote(char *str, char quote, size_t *j)
{
	while (str[*j] && str[*j] != quote)
		(*j)++;
	return ((*j) += 2, ft_substr(str, 0, *j - 2));
}

void	print_token(t_token *lst)
{
	while (lst)
	{
		if (lst->type == WORD || lst->type == Q_WORD)
			printf("%s\n", lst->content);
		if (lst->type == I_FILE)
			printf("Infile\n");
		if (lst->type == O_FILE_APPEND)
			printf("Append\n");
		if (lst->type == O_FILE_TRUNC)
			printf("Truncate\n");
		if (lst->type == HEREDOC)
			printf("Heredoc\n");
		if (lst->type == PIPE)
			printf("Pipe\n");
		lst = lst->next;
	}
}

void	parser(t_token *lst)
{
	
}

void	lexer(char *str)
{
	size_t	i;
	size_t	j;
	t_token	*lst;

	i = 0;
	j = 0;
	lst = NULL;
	while (str[i])
	{
		if (str[i] == ' ')
			;
		else if (str[i] == '>' && str[i + 1] == '>')
		{
			addback_token(&lst, O_FILE_APPEND, NULL);
			i++;
		}
		else if (str[i] == '<' && str[i + 1] == '<')
		{
			addback_token(&lst, HEREDOC, NULL);
			i++;
		}
		else if (str[i] == '\'')
			addback_token(&lst, Q_WORD, quote(str + i + 1, '\'', &j));
		else if (str[i] == '\"')
			addback_token(&lst, Q_WORD, quote(str + i + 1, '\"', &j));
		else if (str[i] == '>')
			addback_token(&lst, O_FILE_TRUNC, NULL);
		else if (str[i] == '<')
			addback_token(&lst, I_FILE, NULL);
		else if (str[i] == '|')
			addback_token(&lst, PIPE, NULL);
		else
			addback_token(&lst, WORD, word(str + i, &j));
		if (j)
			i += j;
		else
			i++;
		j = 0;
	}
	print_token(lst);
}
