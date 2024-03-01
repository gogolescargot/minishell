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
		return (ft_strdup(""));
	return (ft_substr(str, *spc - len, len));
}

void	print_error(int code)
{
	if (code == 1)
		ft_putstr_fd("Synthax error: Missing Pipe Argument\n", 2);
	else if (code == 2)
		ft_putstr_fd("Synthax error: Missing argument or quote\n", 2);
}

void	lexer_error(t_data *data)
{
	if (check_token(data->tokens) != 0)
	{
		print_error(check_token(data->tokens));
		g_exit_code = 1;
		token_clear(&data->tokens, ft_free);
		data->tokens = NULL;
	}
}

t_token	*lexer_utils(t_data *data, enum e_tokentype type, size_t *i)
{
	t_token	*token;
	char	*word;

	if (type == PIPE)
		token = token_new(type, NULL);
	else
	{
		word = tokenizer(data->line, i);
		if (!word)
			secure_exit(&data, 1);
		token = token_new(type, word);
	}
	return (token);
}

/*
 * Lexical analyzer
 *
 * @param str A string to analyze
 * @return A linked list of all the splited tokens in the string
 */

void	lexer(t_data *data)
{
	size_t				i;
	t_token				*token;
	enum e_tokentype	type;

	i = 0;
	while (data->line[i])
	{
		type = is_operator(data->line + i);
		if (type == O_FILE_APPEND || type == HEREDOC)
			i += 2;
		else if (type != WORD)
			i += 1;
		if (type != TOKEN_NONE)
		{
			token = lexer_utils(data, type, &i);
			if (!token)
				secure_exit(&data, 1);
			token_addback(&data->tokens, token);
		}
	}
	lexer_error(data);
}
