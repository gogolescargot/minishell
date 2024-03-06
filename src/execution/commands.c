/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 11:05:00 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/26 11:05:00 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	commands_execute(t_data *data)
{
	t_redir	redir;
	pid_t	pid;
	size_t	i;

	pid = -1;
	i = -1;
	redir_init(&redir, data);
	while (data->cmd && data->cmd[++i])
	{
		if (i == 0)
			redir.fdin = redir.file_fdin;
		dup2(redir.fdin, STDIN_FILENO);
		ft_close(redir.fdin);
		if (data->cmd[i + 1] == NULL)
			redir.fdout = redir.file_fdout;
		else if (redir_pipe(&redir))
			break ;
		dup2(redir.fdout, STDOUT_FILENO);
		ft_close(redir.fdout);
		if (is_builtin(data->cmd[i][0]) != BUILTIN_NONE)
			exec_builtin(data, i, redir, &pid);
		else
			exec_bin(data, i, redir, &pid);
	}
	redir_end(redir, pid);
}

int	command_fill(t_token **tokens, char ***cmd, t_list *envp_lst)
{
	size_t	i;

	i = 0;
	*cmd = ft_calloc(get_cmd_size(*tokens) + 1, sizeof(char *));
	while (*tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == WORD)
		{
			if (is_builtin((*tokens)->content) != BUILTIN_NONE)
				(*cmd)[i] = ft_strdup((*tokens)->content);
			else if (i == 0)
				(*cmd)[i] = get_cmd_path((*tokens)->content, envp_lst);
			else
				(*cmd)[i] = ft_strdup((*tokens)->content);
			if (!(*cmd)[i])
				return (1);
			i++;
		}
		*tokens = (*tokens)->next;
	}
	return (0);
}

int	commands_fill(t_data data)
{
	size_t	i;

	i = 0;
	while (data.tokens)
	{
		if (data.tokens->type == WORD)
		{
			if (command_fill(&data.tokens, &data.cmd[i++], data.envp_lst))
				return (1);
		}
		if (data.tokens)
			data.tokens = data.tokens->next;
	}
	return (0);
}

void	commands_clear(char ****cmd)
{
	size_t	i;

	i = 0;
	if (!(*cmd))
		return ;
	while ((*cmd)[i])
	{
		ft_arrayclear((*cmd)[i]);
		(*cmd)[i] = NULL;
		i++;
	}
	ft_free(*cmd);
	(*cmd) = NULL;
}
