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

#include "../../inc/minishell.h"

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
 * @param envp_lst A linked list that contain the environement variables
 */

int	expand_content_env(t_expand *expand, t_data *data)
{
	char	*env;

	env = get_env(expand->old + expand->i + 1, data->envp_lst);
	if (!env)
		return (1);
	ft_strlcat(expand->new, env, content_len(expand->old, data) + 1);
	expand->j += ft_strlen(env);
	expand->i += getenv_skip(expand->old + expand->i + 1) + 1;
	free(env);
	return (0);
}

/*
 * Expand the content with quote interpretation
 * and environment variables expanded
 *
 * @param str A string to update
 * @param envp_lst A linked list that contain the environement variables
 * @return A pointer to the new updated string
 */

char	*expand_content(char *str, t_data *data)
{
	t_expand	expand;

	expand.i = 0;
	expand.j = 0;
	expand.quoted = 0;
	expand.old = str;
	expand.new = ft_calloc(content_len(str, data) + 1, sizeof(char));
	if (!expand.new)
		return (NULL);
	while (expand.old[expand.i])
	{
		is_quoted(str[expand.i], &expand.quoted);
		if (expand.old[expand.i] == '$' && expand.quoted < 2)
		{
			if (expand_content_env(&expand, data))
				return (free(expand.new), NULL);
		}
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

void	expander(t_data *data)
{
	char	*temp;
	t_token	*current;

	current = data->tokens;
	while (current)
	{
		if (current->content != NULL)
		{
			temp = current->content;
			current->content = expand_content(current->content, data);
			free(temp);
			if (!current->content)
				secure_exit(&data, 1);
		}
		current = current->next;
	}
}
