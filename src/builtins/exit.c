/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:43:18 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:43:18 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Display exit error message
 *
 * @param str An error message
 */

void	exit_error(char *str, t_data *data)
{
	ft_putstr_fd("exit: ", 2);
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": Numeric argument required\n", 2);
	}
	else
		ft_putstr_fd("Too many arguments\n", 2);
	secure_exit(&data, 2);
}

/*
 * Convert the string error code, check the exit function argument and
 * update the error code pointer R
 *
 * @param nptr An exit code number in strig format
 * @param r A pointer to the exit code number
 * @return True if the argument is well right, otherwise return false
 */

bool	exit_check(char *nptr, size_t *r)
{
	int		i;
	int		m;

	i = 0;
	m = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (!nptr[i])
		return (*r = 2, false);
	if ((nptr[i] == 45 || nptr[i] == 43) && nptr[i++] == 45)
		m = -1;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		*r = nptr[i++] - 48 + (*r * 10);
		if ((*r - 1 > LONG_MAX && m == -1) || (*r > LONG_MAX && m == 1))
			return (*r = 2, false);
	}
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i])
		return (*r = 2, false);
	return (true);
}

/*
 * Exit the program with the right error code
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @return 0 on success, 1 in case of error
 */

int	ft_exit(char **cmd, t_data *data)
{
	size_t	exit_code;
	bool	check;

	if (!cmd)
		return (1);
	printf("exit\n");
	if (!cmd[1])
		secure_exit(&data, g_exit_code);
	exit_code = 0;
	check = exit_check(cmd[1], &exit_code);
	if (!check)
		exit_error(cmd[1], data);
	else if (check && cmd[2])
		exit_error(NULL, data);
	else if (check)
		secure_exit(&data, exit_code);
	return (1);
}
