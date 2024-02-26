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

void	commands_execute(char ***cmd, t_token *tokens, t_list *envp_lst)
{
	t_redir			redir;
	pid_t			pid;
	char			**envp;

	pid = -1;
	redirection_init(&envp, envp_lst, &redir);
	if (redirection_in(&redir, tokens) || redirection_out(&redir, tokens))
		return ;
	while (*cmd)
	{
		dup2(redir.fdin, STDIN_FILENO);
		ft_close(redir.fdin);
		if (*(cmd + 1) && redirection_pipe(&redir))
			break ;
		if (*(cmd + 1) == NULL)
			redir.fdout = redir.file_fdout;
		dup2(redir.fdout, STDOUT_FILENO);
		ft_close(redir.fdout);
		if (is_builtin(*(cmd)[0]) != BUILTIN_NONE)
			exec_builtin(*cmd, envp_lst, is_builtin(*(cmd)[0]));
		else
			exec_bin(*cmd, envp, redir, &pid);
		cmd++;
	}
	redirection_end(&envp, redir);
	wait_process(pid);
}

void	command_fill(t_token **tokens, char ***cmd, t_list *envp_lst)
{
	size_t	i;

	i = 0;
	*cmd = ft_calloc(get_cmd_size(*tokens) + 1, sizeof(char *));
	while (*tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == WORD
			&& is_builtin((*tokens)->content) != BUILTIN_NONE)
			(*cmd)[i++] = ft_strdup((*tokens)->content);
		else if ((*tokens)->type == WORD
			&& i == 0)
			(*cmd)[i++] = get_cmd_path((*tokens)->content, envp_lst);
		else if ((*tokens)->type == WORD)
			(*cmd)[i++] = ft_strdup((*tokens)->content);
		*tokens = (*tokens)->next;
	}
}

void	commands_fill(t_token *tokens, t_list *envp_lst, char ****cmd)
{
	size_t	i;

	i = 0;
	while (tokens)
	{
		if (tokens->type == WORD)
			command_fill(&tokens, &(*cmd)[i++], envp_lst);
		if (tokens)
			tokens = tokens->next;
	}
}

void	commands_clear(char ****cmd)
{
	size_t	i;

	i = 0;
	while ((*cmd)[i])
	{
		ft_arrayclear((*cmd)[i]);
		(*cmd)[i] = NULL;
		i++;
	}
	ft_free(*cmd);
	(*cmd) = NULL;
}
