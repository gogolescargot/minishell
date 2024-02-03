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

size_t	getenv_len(char *str)
{
	char	*env;

	env = getenv(str);
	if (!env)
		return (0);
	return (ft_strlen(env));
}

char	*getenv_name(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]) && str[i] != '$')
		i++;
	return (ft_substr(str, 0, i));
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
			len += getenv_len(getenv_name(str + i + 1));
			i += getenv_skip(str + i + 1) + 1;
		}
		else if ((str[i] == '\'' && (quoted == 2 || quoted == 0))
			|| (str[i] == '\"' && (quoted == 1 || quoted == 0)))
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

/* Rewrite it with quote interpretation and environment variables */

char	*handle_content(char *str)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*new_content;
	int		quoted;

	i = 0;
	j = 0;
	quoted = 0;
	len = content_len(str);
	new_content = ft_calloc(len + 1, sizeof(char));
	if (!new_content)
		return (NULL);
	while (str[i])
	{
		is_quoted(str[i], &quoted);
		if (str[i] == '$' && quoted < 2)
		{
			if (getenv(getenv_name(str + i + 1)))
			{
				ft_strlcat(new_content,
					getenv(getenv_name(str + i + 1)), len + 1);
				j += ft_strlen(getenv(getenv_name(str + i + 1)));
			}
			i += getenv_skip(str + i + 1) + 1;
		}
		else if ((str[i] == '\'' && (quoted == 2 || quoted == 0))
			|| (str[i] == '\"' && (quoted == 1 || quoted == 0)))
			i++;
		else
		{
			new_content[j] = str[i];
			i++;
			j++;
		}
	}
	return (new_content);
}

/* Browse all the node with content and extend it */

t_token	*expander(t_token *lst)
{
	while (lst)
	{
		if (lst->content != NULL)
		{
			printf("%s\n", handle_content(lst->content));
		}
		lst = lst->next;
	}
	return (lst);
}
