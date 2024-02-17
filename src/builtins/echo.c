/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:42:46 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:42:46 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Echo the STRING(s) to standard output
 *
 * @param cmd An array of strings that contains all of the environment variables
 * @return 0 on success, 1 in case of error
 */

int	ft_echo(char **cmd)
{
	bool	nl;
	size_t	i;

	nl = true;
	i = 1;
	if (!cmd)
		return (1);
	if (!ft_strncmp(cmd[i], "-n", 3))
	{
		nl = false;
		i++;
	}
	while (cmd[i])
	{
		if (printf("%s", cmd[i]) == -1)
			return (1);
		i++;
		if (cmd[i] && printf(" ") == -1)
			return (1);
	}
	if (nl && printf("\n") == -1)
		return (1);
	return (0);
}
