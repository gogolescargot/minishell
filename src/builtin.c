/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:32:39 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/14 20:32:39 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	echo(char **cmd)
{
	bool	nl;
	int		i;

	nl = true;
	i = 1;
	if (!ft_strncmp(cmd[i], "-n", 3))
	{
		nl = false;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
		if (cmd[i])
			printf(" ");
	}
	if (nl)
		printf("\n");
}
