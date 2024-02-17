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
 * otherwise enumeration token type NONE
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
	else if (str[0] != ' ')
		return (WORD);
	return (NONE);
}

/*
 * Truncate a string to get a word ignoring space and consider quotes
 *
 * @param str A string to truncate
 * @return A pointer to the new word, otherwise NULL
 */

char	*word(char *str, size_t *spc)
{
	size_t	len;
	int		quoted;

	len = 0;
	quoted = 0;
	while (str[*spc] && is_space(str[*spc]))
		(*spc)++;
	while (str[*spc] && !(((is_operator(str + *spc) != NONE
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
			addback_token(&lst, type, NULL);
		else if (type != NONE)
			addback_token(&lst, type, word(str, &i));
	}
	if (check_token(lst) != 0)
		print_error(check_token(lst));
	print_token(lst);
	return (lst);
}
