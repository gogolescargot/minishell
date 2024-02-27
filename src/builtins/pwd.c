/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:42:58 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:42:58 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Print working directory
 *
 * @return 0 on success, 1 in case of error
 */

int	ft_pwd(char **cmd)
{
	char	cwd[4096];

	if (cmd[1])
	{
		ft_putstr_fd("pwd: Too many arguments\n", 2);
		return (1);
	}
	if (getcwd(cwd, 4096))
	{
		if (printf("%s\n", cwd) == -1)
			return (1);
		return (0);
	}
	return (1);
}
