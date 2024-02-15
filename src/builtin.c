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

int	ft_env(char **envp)
{
	size_t	i;

	i = 0;
	if (!envp)
		return (1);
	while (envp[i])
	{
		if (printf("%s\n", envp[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, 4096))
	{
		if (printf("%s\n", cwd) == -1)
			return (1);
		return (0);
	}
	return (1);
}

void	exit_error(char *str)
{
	ft_putstr_fd("exit: ", 2);
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(": Numeric argument required\n", 2);
		return ;
	}
	ft_putstr_fd("Too many arguments\n", 2);
	exit(2);
}

bool	check_exit(char *nptr, size_t *r)
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

int	ft_exit(char **cmd)
{
	size_t	exit_code;
	bool	check;

	if (!cmd)
		return (1);
	printf("exit\n");
	if (!cmd[1])
		exit(g_exit_code);
	exit_code = 0;
	check = check_exit(cmd[1], &exit_code);
	if (!check)
		exit_error(cmd[1]);
	else if (check && cmd[2])
		exit_error(NULL);
	else if (check)
		exit(exit_code);
	return (1);
}

char	*get_envp(char **envp, char *key)
{
	size_t		i;
	size_t		len;

	i = 0;
	len = ft_strlen(key);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	update_envp(char **envp, char *key, char *value)
{
	size_t		i;
	size_t		len;
	char		*str;

	i = 0;
	len = ft_strlen(key);
	str = ft_calloc(len + ft_strlen(value) + 2, sizeof(char));
	ft_strlcpy(str, key, len + 1);
	str[len] = '=';
	ft_strlcpy(str + len + 1, value, ft_strlen(value) + 1);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			break ;
		i++;
	}
	if (!envp[i])
		return (1);
	envp[i] = str;
	return (0);
}

void	cd_error(char *path, char *str)
{
	ft_putstr_fd("cd: ", 2);
	if (!path)
	{
		ft_putstr_fd(str, 2);
		return ;
	}
	ft_putstr_fd(path, 2);
	ft_putstr_fd(str, 2);
}

bool	check_cd_folder(char *path)
{
	struct stat	info;

	if (stat(path, &info) == -1)
	{
		cd_error(path, ": Stat error\n");
		return (false);
	}
	if (S_ISDIR(info.st_mode) == 0)
	{
		cd_error(path, ": Not a directory\n");
		return (false);
	}
	return (true);
}

bool	check_cd_access(char *path)
{
	if (access(path, F_OK | X_OK) == 0)
		return (true);
	else if (access(path, F_OK) == -1)
		cd_error(path, ": No such file or directory\n");
	else if (access(path, X_OK) == -1)
		cd_error(path, ": Permission denied\n");
	return (false);
}

bool	check_cd(char **cmd)
{
	if (!cmd)
		return (false);
	if (cmd[2])
		return (cd_error(NULL, "Too many arguments\n"), false);
	if (!cmd[1])
		return (true);
	if (!check_cd_access(cmd[1]) || !check_cd_folder(cmd[1]))
		return (false);
	return (true);
}

int	ft_cd(char **cmd, char **envp)
{
	char	path[4096];

	if (!check_cd(cmd))
		return (1);
	update_envp(envp, "OLDPWD", getcwd(path, 4096));
	if (!cmd[1])
		chdir(get_envp(envp, "HOME"));
	else
		chdir(cmd[1]);
	update_envp(envp, "PWD", getcwd(path, 4096));
	return (0);
}
