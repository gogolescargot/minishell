/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:52:29 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:52:29 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Truncate a string to get a word ignoring space and consider quotes
 *
 * @param str A string to truncate
 * @return A pointer to the new word, otherwise NULL
 */

char	*tokenizer(char *str, size_t *spc)
{
	size_t	len;
	int		quoted;

	len = 0;
	quoted = 0;
	while (str[*spc] && is_space(str[*spc]))
		(*spc)++;
	while (str[*spc] && !(((is_operator(str + *spc) != TOKEN_NONE
					&& is_operator(str + *spc) != WORD)
				|| is_space(str[*spc])) && !quoted))
	{
		is_quoted(str[*spc], &quoted);
		len++;
		(*spc)++;
	}
	if (!len)
		return (NULL);
	return (ft_substr(str, *spc - len, len));
}

void	print_token(t_token *lst)
{
	while (lst)
	{
		if (lst->type == WORD || lst->type == Q_WORD)
			printf("Word :%s\n", lst->content);
		if (lst->type == I_FILE)
			printf("Input :%s\n", lst->content);
		if (lst->type == O_FILE_APPEND)
			printf("Append :%s\n", lst->content);
		if (lst->type == O_FILE_TRUNC)
			printf("Trunc :%s\n", lst->content);
		if (lst->type == HEREDOC)
			printf("Heredoc :%s\n", lst->content);
		if (lst->type == PIPE)
			printf("Pipe :%s\n", lst->content);
		lst = lst->next;
	}
}

void	print_error(int code)
{
	if (code == 1)
		printf("Missing Pipe Argument or quotes\n");
	else if (code == 2)
		printf("Missing argument or quote\n");
	else if (code == 3)
		printf("Token error\n");
	else if (code == -1)
		printf("Malloc error\n");
}

/*
 * Lexical analyzer
 *
 * @param str A string to analyze
 * @return A linked list of all the splited tokens in the string
 */

t_token	*lexer(char *str)
{
	size_t				i;
	t_token				*lst;
	enum e_tokentype	type;

	i = 0;
	lst = NULL;
	while (str[i])
	{
		type = is_operator(str + i);
		if (type == O_FILE_APPEND || type == HEREDOC)
			i += 2;
		else if (type != WORD)
			i += 1;
		if (type == PIPE)
			token_addback(&lst, type, NULL);
		else if (type != TOKEN_NONE)
			token_addback(&lst, type, tokenizer(str, &i));
	}
	if (check_token(lst) != 0)
		print_error(check_token(lst));
	// print_token(lst);
	return (lst);
}
