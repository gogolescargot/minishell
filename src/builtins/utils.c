/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:32:39 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/14 20:32:39 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Check if the command is a builtin function
 *
 * @param str A command string
 * @return True if the string is a builtin function, otherwise False
 */

bool	is_builtin(char *str)
{
	if (!ft_strncmp(str, "echo", 5))
		return (true);
	else if (!ft_strncmp(str, "cd", 3))
		return (true);
	else if (!ft_strncmp(str, "pwd", 4))
		return (true);
	else if (!ft_strncmp(str, "export", 7))
		return (true);
	else if (!ft_strncmp(str, "unset", 6))
		return (true);
	else if (!ft_strncmp(str, "env", 4))
		return (true);
	else if (!ft_strncmp(str, "exit", 5))
		return (true);
	return (false);
}

/*
 * Compare two strings in an environment variable context
 *
 * @param s1 A first string to compare
 * @param s1 A second string to compare
 * @return True if the two strings are the same
 * and terminated by equals, False otherwise
 */

bool	is_env(char *s1, char *s2)
{
	size_t	len;

	len = ft_strlen(s2);
	return (ft_strncmp(s1, s2, len) == 0 && s1[len] == '=');
}

/*
 * Update the value assigned to a key in an environment array
 *
 * @param envp A linked list that contain the environement variables
 * @param key A key to find
 * @param value A new value to set for the key
 * @return 0 if the key is successfully updated, otherwise 1
 */

void	update_env(t_list *envp, char *key, char *value)
{
	size_t		len;
	char		*str;

	len = ft_strlen(key);
	str = ft_calloc(len + ft_strlen(value) + 2, sizeof(char));
	if (!str)
		return ;
	ft_strlcpy(str, key, len + 1);
	str[len] = '=';
	ft_strlcpy(str + len + 1, value, ft_strlen(value) + 1);
	while (envp)
	{
		if (is_env(envp->content, key))
			break ;
		envp = envp->next;
	}
	if (!envp)
		return ;
	free(envp->content);
	envp->content = str;
}