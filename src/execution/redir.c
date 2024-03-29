/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:43:55 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/26 10:43:55 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redir_init(t_redir *redir, t_data *data)
{
	(*redir).fdin = -1;
	(*redir).fdout = -1;
	(*redir).file_fdin = -1;
	(*redir).file_fdout = -1;
	(*redir).tmp_fdin = dup(STDIN_FILENO);
	(*redir).tmp_fdout = dup(STDOUT_FILENO);
	redir_in(redir, data->tokens);
	redir_out(redir, data->tokens);
}

void	redir_end(t_redir redir, int pid)
{
	dup2(redir.tmp_fdin, STDIN_FILENO);
	dup2(redir.tmp_fdout, STDOUT_FILENO);
	ft_close(redir.fdin);
	ft_close(redir.fdout);
	ft_close(redir.tmp_fdin);
	ft_close(redir.tmp_fdout);
	unlink(".here_doc");
	wait_process(pid);
}

int	redir_in(t_redir *redir, t_token *tokens)
{
	int		fd;
	bool	trigger;

	trigger = false;
	fd = -1;
	while (tokens)
	{
		if (tokens->type == I_FILE || tokens->type == HEREDOC)
		{
			if (fd != -1)
				ft_close_unlink(fd, ".here_doc");
			if (tokens->type == I_FILE)
				fd = open(tokens->content, O_RDONLY);
			else
				fd = heredoc(tokens->content);
			if (fd < 0)
				return (handle_error(tokens->content, 1));
			(*redir).file_fdin = fd;
			trigger = true;
		}
		tokens = tokens->next;
	}
	if (!trigger)
		(*redir).file_fdin = dup(redir->tmp_fdin);
	return (0);
}

int	redir_pipe(t_redir *redir)
{
	int	fd[2];

	if (pipe(fd) < 0)
		return (handle_error("pipe", 1));
	(*redir).fdin = fd[0];
	(*redir).fdout = fd[1];
	return (0);
}

int	redir_out(t_redir *redir, t_token *tokens)
{
	int		fd;
	bool	trigger;

	trigger = false;
	fd = -1;
	while (tokens)
	{
		if (tokens->type == O_FILE_TRUNC || tokens->type == O_FILE_APPEND)
		{
			if (fd != -1)
				ft_close(fd);
			if (tokens->type == O_FILE_TRUNC)
				fd = open(tokens->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd = open(tokens->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (handle_error(tokens->content, 1));
			(*redir).file_fdout = fd;
			trigger = true;
		}
		tokens = tokens->next;
	}
	if (!trigger)
		(*redir).file_fdout = dup(redir->tmp_fdout);
	return (0);
}
