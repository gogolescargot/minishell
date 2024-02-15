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
	size_t	i;

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
	g_exit_code = 0;
}

void	env(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	g_exit_code = 0;
}

void	pwd(char **envp)
{
	size_t	i;
	size_t	j;
	char	*str;

	i = 0;
	j = 0;
	while (envp && envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		str = ft_substr(envp[i], 0, j);
		if (ft_strncmp(str, "PWD", ft_strlen(str)) == 0)
		{
			free(str);
			printf("%s\n", envp[i] + j + 1);
			g_exit_code = 0;
			return ;
		}
		free(str);
		i++;
	}
}

int	check_exit(char *nptr)
{
	int		i;
	int		m;
	size_t	r;

	i = 0;
	r = 0;
	m = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (!nptr[i])
		return (printf("exit\n%s: numeric argument required\n", nptr), 2);
	if ((nptr[i] == 45 || nptr[i] == 43) && nptr[i++] == 45)
		m = -1;
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		r = nptr[i++] - 48 + (r * 10);
		if ((r - 1 > LONG_MAX && m == -1) || (r > LONG_MAX && m == 1))
			return (printf("exit\n%s: numeric argument required\n", nptr), 2);
	}
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i])
		return (printf("exit\n%s: numeric argument required\n", nptr), 2);
	return (r * m);
}

void	ft_exit(char **cmd)
{
	if (cmd[2])
	{
		printf("exit: too many arguments");
		return ;
	}
	if (!cmd[1])
		exit(g_exit_code);
	else
		exit(check_exit(cmd[1]));
}
