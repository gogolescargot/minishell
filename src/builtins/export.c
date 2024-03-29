/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:40:41 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:40:41 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Display a export error message
 *
 * @param str An error message
 */

void	export_error(char *str)
{
	if (!str)
	{
		ft_putstr_fd("export: Malloc error\n", 2);
	}
	else
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("': Not a valid identifier\n", 2);
	}
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
	char	*name;

	*current = envp;
	i = -1;
	while (cmd[++i] && cmd[i] != '=' && !is_space(cmd[i]))
	{
		if (is_operator(cmd + i) != WORD || ft_isdigit(cmd[i])
			|| is_quote(cmd[i]))
			return (-1);
	}
	if (cmd[i] == '=' && i == 0)
		return (-1);
	else if (!cmd[i])
		return (0);
	name = ft_substr(cmd, 0, i);
	if (!name)
		return (-2);
	while (*current)
	{
		if (is_env((*current)->content, name))
			return (free(name), 1);
		*current = (*current)->next;
	}
	return (free(name), 2);
}

void	export_replace(char **cmd, t_list *target, size_t i)
{
	free(target->content);
	target->content = ft_strdup(cmd[i]);
}

/*
 * Export one or multiple environment variables
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_export(char **cmd, t_list **envp)
{
	int		check;
	t_list	*target;
	size_t	i;

	i = 1;
	if (!cmd)
		return (1);
	if (!cmd[i])
		return (export_print(*envp));
	while (cmd[i])
	{
		target = NULL;
		check = export_check(cmd[i], *envp, &target);
		if (check == -2)
			return (export_error(NULL), 1);
		else if (check == -1)
			return (export_error(cmd[i]), 1);
		else if (check == 1)
			export_replace(cmd, target, i);
		else if (check == 2)
			ft_lstadd_back(envp, ft_lstnew(ft_strdup(cmd[i])));
		i++;
	}
	return (0);
}
