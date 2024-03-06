/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 06:04:39 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 06:04:39 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	if (str[i] == '?')
		return (1);
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
	if (i == 0)
		return ("");
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
	char	*env_name;
	size_t	len;

	env_name = getenv_name(str);
	len = ft_strlen(env_name);
	if (!env_name)
		return (NULL);
	if (!env_name[0])
		return (ft_strdup("$"));
	if (!ft_strncmp(env_name, "?", 1))
		return (free(env_name), ft_itoa(g_exit_code));
	while (envp)
	{
		if (is_env(envp->content, env_name))
			return (free(env_name), ft_strdup(envp->content + len + 1));
		envp = envp->next;
	}
	free(env_name);
	return (ft_strdup(""));
}
