/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:44:10 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/26 10:44:10 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_builtin(char **args, t_data *data,
	t_redir redir, pid_t *pid)
{
	enum e_builtin	type;

	type = is_builtin(args[0]);
	*pid = fork();
	if (*pid == 0)
	{
		close_fds_redir(redir);
		if (type == ECHO)
			g_exit_code = ft_echo(args);
		else if (type == CD)
			g_exit_code = ft_cd(args, data->envp_lst);
		else if (type == PWD)
			g_exit_code = ft_pwd();
		else if (type == EXPORT)
			g_exit_code = ft_export(args, data->envp_lst);
		else if (type == UNSET)
			g_exit_code = ft_unset(args, data->envp_lst);
		else if (type == ENV)
			g_exit_code = ft_env(data->envp_lst);
		else if (type == EXIT)
			g_exit_code = ft_exit(args, data);
		secure_exit(&data, g_exit_code);
	}
}

void	builtin_execute(char **cmd, t_data *data,
	enum e_builtin type)
{
	t_redir	redir;

	if (redir_init(&redir, data))
		return ;
	dup2(redir.fdin, STDIN_FILENO);
	ft_close(redir.fdin);
	redir.fdout = redir.file_fdout;
	dup2(redir.fdout, STDOUT_FILENO);
	ft_close(redir.fdout);
	if (type == ECHO)
		g_exit_code = ft_echo(cmd);
	else if (type == CD)
		g_exit_code = ft_cd(cmd, data->envp_lst);
	else if (type == PWD)
		g_exit_code = ft_pwd();
	else if (type == EXPORT)
		g_exit_code = ft_export(cmd, data->envp_lst);
	else if (type == UNSET)
		g_exit_code = ft_unset(cmd, data->envp_lst);
	else if (type == ENV)
		g_exit_code = ft_env(data->envp_lst);
	else if (type == EXIT)
		g_exit_code = ft_exit(cmd, data);
	redir_end(redir, -1);
}

void	exec_bin(char **cmd, t_data *data, t_redir redir, pid_t *pid)
{
	int	error_code;

	*pid = fork();
	if (*pid == 0)
	{
		close_fds_redir(redir);
		execve(cmd[0], cmd, data->envp);
		error_code = errno;
		perror(cmd[0]);
		secure_exit(&data, error_code);
	}
}

void	wait_process(int pid)
{
	int	status;
	int	ret_value;
	int	cur_pid;

	if (pid == -1)
		return ;
	while (true)
	{
		cur_pid = wait(&status);
		if (cur_pid == -1)
			break ;
		if (cur_pid != pid)
			continue ;
		if (WIFEXITED(status))
			ret_value = WEXITSTATUS(status);
		else
		{
			g_exit_code = 128 + WTERMSIG(status);
			return ;
		}
	}
	g_exit_code = ret_value;
}

void	execution(t_data *data)
{
	if (!data->tokens)
		return ;
	data->cmd = ft_calloc(get_cmd_nbr(data->tokens) + 1, sizeof(char **));
	if (!data->cmd)
		secure_exit(&data, 1);
	if (commands_fill(*data))
		secure_exit(&data, 1);
	if (get_cmd_nbr(data->tokens) == 1
		&& is_builtin(data->cmd[0][0]) != BUILTIN_NONE)
		builtin_execute(data->cmd[0], data, is_builtin(data->cmd[0][0]));
	else
		commands_execute(data);
	commands_clear(&data->cmd);
}
