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

bool	cd_home(t_list *envp_lst)
{
	char	*home;

	home = get_env("HOME", envp_lst);
	if (!home)
	{
		cd_error(NULL, NULL);
		return (false);
	}
	if (!home[0])
	{
		cd_error(NULL, "HOME not set\n");
		free(home);
		return (false);
	}
	chdir(home);
	if (errno != 0)
	{
		perror("cd");
		free(home);
		return (false);
	}
	free(home);
	return (true);
}

/*
 * Change directory
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_cd(char **cmd, t_list **envp_lst)
{
	char	path[4096];

	if (!cd_check(cmd))
		return (1);
	if (!update_env(*envp_lst, "OLDPWD", getcwd(path, 4096)))
		return (cd_error(NULL, NULL), 1);
	if (!cmd[1])
	{
		if (cd_home(*envp_lst) == false)
			return (1);
	}
	else
	{
		chdir(cmd[1]);
		if (errno != 0)
			return (cd_error(cmd[1], NULL), 1);
	}
	if (!update_env(*envp_lst, "PWD", getcwd(path, 4096)))
		return (cd_error(NULL, NULL), 1);
	return (0);
}
