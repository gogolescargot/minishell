/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:01:45 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/26 11:01:45 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**env_lst_to_str(t_list *envp_lst)
{
	char	**envp;
	size_t	i;

	i = 0;
	envp = ft_calloc(ft_lstsize(envp_lst) + 1, sizeof(char *));
	while (envp_lst)
	{
		envp[i] = envp_lst->content;
		envp_lst = envp_lst->next;
		i++;
	}
	return (envp);
}

bool	is_absolute_path(char *cmd)
{
	if (cmd[0] == '/')
		return (true);
	else if (cmd[0] == '.' && cmd[1] == '/')
		return (true);
	else if (cmd[0] == '.' && cmd[1] == '.' && cmd[2] == '/')
		return (true);
	return (false);
}

char	*get_cmd_path(char *cmd, t_list *envp_lst)
{
	char	**all_path;
	char	*path;
	char	*exec;
	int		i;

	if (is_absolute_path(cmd))
		return (ft_strdup(cmd));
	i = -1;
	all_path = ft_split(get_env("PATH", envp_lst), ':');
	while (all_path && all_path[++i])
	{
		path = ft_strjoin(all_path[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (access(exec, (F_OK | X_OK)) == 0)
		{
			ft_arrayclear(all_path);
			return (exec);
		}
		free(exec);
	}
	ft_arrayclear(all_path);
	return (ft_strdup(cmd));
}
