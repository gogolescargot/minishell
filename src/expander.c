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

/*
 * Increment an index to skip the environment variable
 *
 * @param str A pointer to the beginning of the environment variable name
 * @return A number of characters to skip
 */

size_t	getenv_skip(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]) && str[i] != '$')
		i++;
	return (i);
}

/*
 * Get a key of an environment variable name without dollar
 *
 * @param str A pointer to the beginning of the environment variable name
 * @return A pointer to a new environment variable name
 */

char	*getenv_name(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] && !is_space(str[i]) && !is_quote(str[i]) && str[i] != '$')
		i++;
	return (ft_substr(str, 0, i));
}

/*
 * Get an environment variable value associated with a name
 *
 * @param str A environment variable name
 * @param envp A linked list that contain the environement variables
 * @return A pointer to environment variable value associated with the name
 */

char	*get_env(char *str, t_list *envp)
{
	if (!ft_strncmp(getenv_name(str), "?", 2))
		return (ft_itoa(g_exit_code));
	while (envp)
	{
		if (is_env(envp->content, getenv_name(str)))
			return (envp->content + ft_strlen(getenv_name(str)) + 1);
		envp = envp->next;
	}
	return (NULL);
}

/*
 * Check if we are between quotes
 *
 * @param c A character to check
 * @param quoted A current state of quotes
 * @return A new state of quotes
 */

bool	is_interpreted_quote(char c, int quoted)
{
	return ((c == '\'' && (quoted == 2 || quoted == 0))
		|| (c == '\"' && (quoted == 1 || quoted == 0)));
}

/*
 * Calculate the size of the new content
 *
 * @param str A string to handle
 * @param envp A linked list that contain the environement variables
 * @return A calculated new lengh
 */

size_t	content_len(char *str, t_list *envp)
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
			len += ft_strlen(get_env(str + i + 1, envp));
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

/*
 * Copy a caracter from the original string to the updated string
 *
 * @param str A string to handle
 * @param new_content A buffer for the new updated string
 * @param i A pointer to a size_t containing
 * the current index of the environment variable in the original string
 * @param j A pointer to a size_t containing
 * the current index of the environment variable in the updated string
 */

void	expand_content_def(t_expand *expand)
{
	expand->new[expand->j] = expand->old[expand->i];
	expand->i++;
	expand->j++;
}

/*
 * Concatenate the environment variable to the new content
 * and update i and j indexes
 *
 * @param expand A structure containing the parameters below
 * @param str A string to handle
 * @param old A buffer for the new updated string
 * @param i A size_t containing
 * the current index of the environment variable in the original string
 * @param j A size_t containing
 * the current index of the environment variable in the updated string
 * @param envp A linked list that contain the environement variables
 */

void	expand_content_env(t_expand *expand, t_list *envp)
{
	if (get_env(expand->old + expand->i + 1, envp))
	{
		ft_strlcat(expand->new, get_env(expand->old + expand->i + 1, envp),
			content_len(expand->old, envp) + 1);
		expand->j += ft_strlen(get_env(expand->old + expand->i + 1, envp));
	}
	expand->i += getenv_skip(expand->old + expand->i + 1) + 1;
}

/*
 * Expand the content with quote interpretation
 * and environment variables expanded
 *
 * @param str A string to update
 * @param envp A linked list that contain the environement variables
 * @return A pointer to the new updated string
 */

char	*expand_content(char *str, t_list *envp)
{
	t_expand	expand;

	expand.i = 0;
	expand.j = 0;
	expand.old = str;
	expand.new = ft_calloc(content_len(str, envp) + 1, sizeof(char));
	expand.quoted = 0;
	if (!expand.new)
		return (NULL);
	while (expand.old[expand.i])
	{
		is_quoted(str[expand.i], &expand.quoted);
		if (expand.old[expand.i] == '$' && expand.quoted < 2)
			expand_content_env(&expand, envp);
		else if (is_interpreted_quote(str[expand.i], expand.quoted))
			expand.i++;
		else
			expand_content_def(&expand);
	}
	return (expand.new);
}

/*
 * Browse all the node with content and extend it
 *
 * @param lst A linked list that contain all the tokens to expand
 * @param envp A linked list that contain the environement variables
 * @return A pointer to the beginning of the LST linked list
 */

t_token	*expander(t_token *lst, t_list *envp)
{
	char	*temp;
	t_token	*current;

	current = lst;
	while (current)
	{
		if (current->content != NULL)
		{
			temp = current->content;
			current->content = expand_content(current->content, envp);
			free(temp);
		}
		current = current->next;
	}
	return (lst);
}
