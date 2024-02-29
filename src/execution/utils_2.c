/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
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
	char	*env;
	int		i;

	if (is_absolute_path(cmd))
		return (ft_strdup(cmd));
	i = -1;
	env = get_env("PATH", envp_lst);
	all_path = ft_split(env, ':');
	ft_free(env);
	while (all_path && all_path[++i])
	{
		path = ft_strjoin(all_path[i], "/");
		exec = ft_strjoin(path, cmd);
		free(path);
		if (access(exec, (F_OK | X_OK)) == 0)
			return (ft_arrayclear(all_path), exec);
		free(exec);
	}
	ft_arrayclear(all_path);
	return (ft_strdup(cmd));
}

int	heredoc(char *limiter)
{
	char	*line;
	int		fd;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			ft_close(fd);
			fd = open(".here_doc", O_RDONLY);
			free(line);
			return (fd);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	ft_close_unlink(fd, ".here_doc");
	return (-1);
}

void	close_fds_redir(t_redir redir)
{
	ft_close(redir.tmp_fdin);
	ft_close(redir.tmp_fdout);
	ft_close(redir.file_fdout);
	ft_close(redir.fdin);
	ft_close(redir.fdout);
}
