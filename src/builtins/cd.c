/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:40:31 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:40:31 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Display a cd error message
 *
 * @param path A path to the file
 * @param str An error message
 */

void	cd_error(char *path, char *str)
{
	ft_putstr_fd("cd: ", 2);
	if (!path && !str)
		ft_putstr_fd("Malloc error\n", 2);
	else if (!path)
		ft_putstr_fd(str, 2);
	else
		perror(path);
}

/*
 * Checks if the command parameters and can be successfully reached.

 * @param cmd An array of strings containing the command itself and the arguments
 * @return True if the given path can be successfully reached, otherwise False
 */

bool	cd_check(char **cmd)
{
	if (!cmd)
		return (false);
	if (cmd[1] && cmd[2])
		return (cd_error(NULL, "Too many arguments\n"), false);
	return (true);
}

/*
 * Change directory
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_cd(char **cmd, t_list *envp)
{
	char	path[4096];
	char	*home;

	if (!cd_check(cmd))
		return (1);
	if (update_env(envp, "OLDPWD", getcwd(path, 4096)))
		return (cd_error(NULL, NULL), 1);
	if (!cmd[1])
	{
		home = get_env("HOME", envp);
		if (!home)
			return (cd_error(NULL, "HOME not set\n"), 1);
		chdir(home);
		if (errno != 0)
			return (cd_error(cmd[1], NULL), 1);
	}
	else
	{
		chdir(cmd[1]);
		if (errno != 0)
			return (cd_error(cmd[1], NULL), 1);
	}
	if (update_env(envp, "PWD", getcwd(path, 4096)))
		return (cd_error(NULL, NULL), 1);
	return (0);
}
