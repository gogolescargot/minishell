/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 04:23:15 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/01 04:23:15 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	getenv_skip(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]) && str[i] != '$')
		i++;
	return (i);
}

char	*getenv_name(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]) && str[i] != '$')
		i++;
	return (ft_substr(str, 0, i));
}

char	*get_env(char *str)
{
	if (!ft_strncmp(getenv_name(str), "?", 2))
		return (ft_itoa(g_exit_code));
	return (getenv(getenv_name(str)));
}

bool	is_interpreted_quote(char c, int quoted)
{
	return ((c == '\'' && (quoted == 2 || quoted == 0))
		|| (c == '\"' && (quoted == 1 || quoted == 0)));
}

/* Calculate the size of the new content */

size_t	content_len(char *str)
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
			len += ft_strlen(get_env(str + i + 1));
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

void	update_content(char *str, char *new_content, size_t *i, size_t *j)
{
	new_content[*j] = str[*i];
	(*i)++;
	(*j)++;
}

/* Concatenate the environment variable to the new content
   and skip the $VAR */

void	update_content_env(char *str, char *new_content, size_t *i, size_t *j)
{
	if (get_env(str + *i + 1))
	{
		ft_strlcat(new_content, get_env(str + *i + 1), content_len(str) + 1);
		*j += ft_strlen(get_env(str + *i + 1));
	}
	*i += getenv_skip(str + *i + 1) + 1;
}

/* Rewrite content with quote interpretation
   and environment variables expanded */

char	*handle_content(char *str)
{
	size_t	i;
	size_t	j;
	char	*new_content;
	int		quoted;

	i = 0;
	j = 0;
	quoted = 0;
	new_content = ft_calloc(content_len(str) + 1, sizeof(char));
	if (!new_content)
		return (NULL);
	while (str[i])
	{
		is_quoted(str[i], &quoted);
		if (str[i] == '$' && quoted < 2)
			update_content_env(str, new_content, &i, &j);
		else if (is_interpreted_quote(str[i], quoted))
			i++;
		else
			update_content(str, new_content, &i, &j);
	}
	return (new_content);
}

/* Browse all the node with content and extend it */

t_token	*expander(t_token *lst)
{
	char	*temp;
	t_token	*current;

	current = lst;
	while (current)
	{
		if (current->content != NULL)
		{
			temp = current->content;
			current->content = handle_content(current->content);
			free(temp);
		}
		current = current->next;
	}
	return (lst);
}
