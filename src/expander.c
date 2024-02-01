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

// char	*remove_quotes(char *str)
// {

// }

// char	*expand_dollar(char *str)
// {

// }

int	get_env_lengh(char *str)
{
	char	*env;

	env = getenv(str);
	if (!env)
		return (0);
	return (ft_strlen(env));
}

char	*get_env_name(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]))
		i++;
	return (ft_substr(str, 0, i));
}

int	get_len_content(char *str)
{
	size_t	i;
	size_t	len;
	int		quoted;

	i = 0;
	len = 0;
	while (str[i])
	{
		is_quoted(str[i], &quoted);
		if ((str[i] == '\'' && (quoted == 2 || quoted == 0)) || (str[i] == '\"' && (quoted == 1 || quoted == 0)))
		{
			i++;
			continue;
		}
		if (quoted < 2 && str[i] == '$')
		{
			len += get_env_lengh(get_env_name(str + i + 1));
			while (str[i] && !is_space(str[i]) && !is_quote(str[i]))
				i++;
			continue;
		}
		len++;
		i++;
	}
	return (len);
}

char	*handle_content(char *str)
{
	// size_t	i;
	size_t	len;
	// int		quoted;

	// i = 0;
	// quoted = 0;
	len = get_len_content(str);
	printf("%zu\n", len);
	// while (str[i])
	// {
	// 	if (quoted < 2 && str[i] == '$')
	// 	{
	// 		get_env_name(str + i + 1);
	// 	}
	// 	i++;
	// }
	return ("str");
}

t_token	*expander(t_token *lst)
{
	while (lst)
	{
		if (lst->content != NULL)
		{
			handle_content(lst->content);
		}
		lst = lst->next;
	}
	return (lst);
}