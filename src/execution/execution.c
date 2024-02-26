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

void	exec_builtin(char **args, t_list *envp_lst, enum e_builtin type)
{
	if (type == ECHO)
		g_exit_code = ft_echo(args);
	else if (type == CD)
		g_exit_code = ft_cd(args, envp_lst);
	else if (type == PWD)
		g_exit_code = ft_pwd();
	else if (type == EXPORT)
		g_exit_code = ft_export(args, envp_lst);
	else if (type == UNSET)
		g_exit_code = ft_unset(args, envp_lst);
	else if (type == ENV)
		g_exit_code = ft_env(envp_lst);
	else if (type == EXIT)
		g_exit_code = ft_exit(args);
}

void	exec_bin(char **cmd, char **envp, t_redir redir, pid_t *pid)
{
	*pid = fork();
	if (*pid == 0)
	{
		ft_close(redir.tmp_fdin);
		ft_close(redir.tmp_fdout);
		ft_close(redir.fdin);
		ft_close(redir.fdout);
		execve(cmd[0], cmd, envp);
		perror(cmd[0]);
		exit(1);
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

void	execution(t_token *tokens, t_list *envp_lst)
{
	char	***cmd;

	if (!tokens)
		return ;
	cmd = ft_calloc(get_cmd_nbr(tokens) + 1, sizeof(char **));
	commands_fill(tokens, envp_lst, &cmd);
	commands_execute(cmd, tokens, envp_lst);
	commands_clear(&cmd);
}
