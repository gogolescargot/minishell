/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 06:58:26 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/19 06:58:26 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

size_t	get_cmd_nbr(t_token *tokens)
{
	size_t	i;

	i = 1;
	while (tokens)
	{
		if (tokens->type == PIPE)
			i++;
		tokens = tokens->next;
	}
	return (i);
}

size_t	get_cmd_size(t_token *tokens)
{
	size_t	i;

	i = 0;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD)
			i++;
		tokens = tokens->next;
	}
	return (i);
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

void	fill_command(t_token **tokens, char ***cmd, t_list *envp_lst)
{
	size_t	i;

	i = 0;
	*cmd = ft_calloc(get_cmd_size(*tokens) + 1, sizeof(char *));
	while (*tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == WORD && !(*tokens)->content[0])
			(*cmd)[i++] = ft_strdup((*tokens)->content);
		else if ((*tokens)->type == WORD
			&& is_builtin((*tokens)->content) == BUILTIN_NONE && i == 0)
			(*cmd)[i++] = get_cmd_path((*tokens)->content, envp_lst);
		else if ((*tokens)->type == WORD)
		{
			(*cmd)[i++] = ft_strdup((*tokens)->content);
		}
		*tokens = (*tokens)->next;
	}
}

void	print_command(char **cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (i == 0)
			printf("Command: %s\n", cmd[i]);
		else
			printf("Argument: %s\n", cmd[i]);
		i++;
	}
}

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

void	handle_error(char *str, int error_code)
{
	if (error_code == -1)
	{
		str = ft_strjoin(str, ": Command not found\n");
		ft_putstr_fd(str, 2);
		free(str);
	}
	else
	{
		errno = error_code;
		perror(str);
	}
}

int	exec_builtin(char **args, t_list *envp_lst, enum e_builtin type)
{
	if (type == ECHO)
		return (ft_echo(args));
	else if (type == CD)
		return (ft_cd(args, envp_lst));
	else if (type == PWD)
		return (ft_pwd());
	else if (type == EXPORT)
		return (ft_export(args, envp_lst));
	else if (type == UNSET)
		return (ft_unset(args, envp_lst));
	else if (type == ENV)
		return (ft_env(envp_lst));
	else if (type == EXIT)
		return (ft_exit(args));
	return (1);
}

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
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
		if (cmd[0])
			execve(cmd[0], cmd, envp);
		perror(cmd[0]);
		exit(1);
	}
}

int	wait_process(pid_t pid)
{
	int	status;
	int	ret_value;
	int	cur_pid;

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
			return (128 + WTERMSIG(status));
	}
	return (ret_value);
}

void	execute_init(char ***envp, t_list *envp_lst, t_redir *redir)
{
	*envp = env_lst_to_str(envp_lst);
	(*redir).tmp_fdin = dup(STDIN_FILENO);
	(*redir).tmp_fdout = dup(STDOUT_FILENO);
}

void	execute_end(char ***envp, t_redir redir)
{
	dup2(redir.tmp_fdin, STDIN_FILENO);
	dup2(redir.tmp_fdout, STDOUT_FILENO);
	ft_close(redir.tmp_fdin);
	ft_close(redir.tmp_fdout);
	ft_free(*envp);
}

int	heredoc(char *limiter)
{
	char	*line;
	int		fd;

	fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror(".here_doc"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (ft_close(fd), unlink(".here_doc"), -1);
		if (!ft_strncmp(line, limiter, ft_strlen(limiter) + 1))
		{
			ft_close(fd);
			fd = open(".here_doc", O_RDONLY);
			free(line);
			return (fd);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	unlink(".here_doc");
	return (-1);
}

void	ft_close_unlink(int fd, char *file)
{
	ft_close(fd);
	unlink(file);
}

void	redirection_in(t_redir *redir, t_token *tokens)
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
				return (perror(tokens->content));
			(*redir).fdin = fd;
			trigger = true;
		}
		tokens = tokens->next;
	}
	if (!trigger)
		(*redir).fdin = dup(redir->tmp_fdin);
}

void	redirection_pipe(t_redir *redir)
{
	int	fd[2];

	if (pipe(fd) < 0)
	{
		perror("pipe");
		exit(1);
	}
	(*redir).fdin = fd[0];
	(*redir).fdout = fd[1];
}

void	redirection_out(t_redir *redir, t_token *tokens)
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
				return (perror(tokens->content));
			(*redir).fdout = fd;
			trigger = true;
		}
		tokens = tokens->next;
	}
	if (!trigger)
		(*redir).fdout = dup(redir->tmp_fdout);
}

void	execute(char ***cmd, t_token *tokens, t_list *envp_lst)
{
	t_redir			redir;
	pid_t			pid;
	char			**envp;

	execute_init(&envp, envp_lst, &redir);
	redirection_in(&redir, tokens);
	while (*cmd)
	{
		dup2(redir.fdin, STDIN_FILENO);
		ft_close(redir.fdin);
		if (*(cmd + 1) == NULL)
			redirection_out(&redir, tokens);
		else
			redirection_pipe(&redir);
		dup2(redir.fdout, STDOUT_FILENO);
		ft_close(redir.fdout);
		if (is_builtin(*(cmd)[0]) != BUILTIN_NONE)
			exec_builtin(*cmd, envp_lst, is_builtin(*(cmd)[0]));
		else
			exec_bin(*cmd, envp, redir, &pid);
		cmd++;
	}
	execute_end(&envp, redir);
	wait_process(pid);
}

void	execution(t_token *tokens, t_list *envp_lst)
{
	size_t	i;
	size_t	nbr_cmd;
	t_token	*current;
	char	***cmd;

	current = tokens;
	if (!tokens)
		return ;
	i = 0;
	nbr_cmd = get_cmd_nbr(tokens);
	cmd = ft_calloc(nbr_cmd + 1, sizeof(char **));
	while (current)
	{
		if (current->type == WORD)
			fill_command(&current, &cmd[i++], envp_lst);
		if (current)
			current = current->next;
	}
	execute(cmd, tokens, envp_lst);
	i = 0;
	while (cmd[i])
	{
		ft_arrayclear(cmd[i]);
		cmd[i] = NULL;
		i++;
	}
	ft_free(cmd);
	cmd = NULL;
}
