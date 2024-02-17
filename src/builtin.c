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

/*
 * Display all environment variables
 *
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_env(t_list *envp)
{
	if (!envp)
		return (1);
	while (envp)
	{
		if (printf("%s\n", (char *)envp->content) == -1)
			return (1);
		envp = envp->next;
	}
	return (0);
}

/*
 * Print working directory
 *
 * @return 0 on success, 1 in case of error
 */

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

/*
 * Display exit error message
 *
 * @param str An error message
 */

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
	check = exit_check(cmd[1], &exit_code);
	if (!check)
		exit_error(cmd[1]);
	else if (check && cmd[2])
		exit_error(NULL);
	else if (check)
		exit(exit_code);
	return (1);
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

/*
 * Display a cd error message
 *
 * @param path A path to the file
 * @param str An error message
 */

void	cd_error(char *path, char *str)
{
	ft_putstr_fd("cd: ", 2);
	if (!path)
	{
		ft_putstr_fd(str, 2);
		return ;
	}
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
	if (cmd[2])
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
	update_env(envp, "OLDPWD", getcwd(path, 4096));
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
	update_env(envp, "PWD", getcwd(path, 4096));
	return (0);
}

/*
 * Display a export error message
 *
 * @param str An error message
 */

void	export_error(char *str)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': Not a valid identifier\n", 2);
}

/*
 * Display all exported variables
 *
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	export_print(t_list *envp)
{
	while (envp)
	{
		if (printf("declare -x %s\n", (char *)envp->content) == -1)
			return (1);
		envp = envp->next;
	}
	return (0);
}

/*
 * Check the synthax and if the environment variable name already exist
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @param current A pointer on the target node
 * @return 0 on success, 1 in case of error
 */

int	export_check(char *cmd, t_list *envp, t_list **current)
{
	size_t	i;

	i = 0;
	*current = envp;
	while (cmd[i] && cmd[i] != '=')
	{
		if (is_space(cmd[i]) || is_operator(cmd + i) != WORD
			|| is_quote(cmd[i]))
			return (-1);
		i++;
	}
	if (!cmd[i] || i == 0)
		return (-1);
	while (*current)
	{
		if (is_env((*current)->content, cmd))
			return (1);
		*current = (*current)->next;
	}
	return (2);
}

/*
 * Export one or multiple environment variables
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_export(char **cmd, t_list *envp)
{
	int		check;
	t_list	*target;
	size_t	i;

	i = 1;
	if (!cmd)
		return (1);
	if (!cmd[i])
		return (export_print(envp));
	while (cmd[i])
	{
		target = NULL;
		check = export_check(cmd[i], envp, &target);
		if (check == -1)
			return (export_error(cmd[i]), 1);
		else if (check == 1)
		{
			free(target->content);
			target->content = cmd[i];
		}
		else if (check == 2)
			ft_lstadd_back(&envp, ft_lstnew(ft_strdup(cmd[i])));
		i++;
	}
	return (0);
}

/*
 * Pop one element from a linked list and reconnect all the pointer correctly

 * @param envp A linked list that contain the environement variables
 * @param current A pointer to the current linked list node
 * @param current A pointer to the previous linked list node
 */

void	unset_pop(t_list *envp, t_list *current, t_list *prev)
{
	if (prev)
	{
		prev->next = current->next;
		free(current->content);
		free(current);
		current = prev->next;
	}
	else
	{
		envp = current->next;
		free(current->content);
		free(current);
		current = envp;
	}
}

/*
 * Utils function of unset
 *
 * @param envp A linked list that contain the environement variables
 * @param current A pointer to the current linked list node
 * @param current A pointer to the previous linked list node
 */

void	unset_utils(t_list *envp, t_list *current, t_list *prev, char *cmd)
{
	while (current)
	{
		if (is_env(current->content, cmd))
		{
			unset_pop(envp, current, prev);
			break ;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/*
 * Unset one or multiple environment variables
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_unset(char **cmd, t_list *envp)
{
	t_list	*current;
	t_list	*prev;
	size_t	i;

	i = 1;
	if (!cmd)
		return (1);
	while (cmd[i])
	{
		prev = NULL;
		current = envp;
		unset_utils(envp, current, prev, cmd[i]);
		i++;
	}
	return (0);
}
