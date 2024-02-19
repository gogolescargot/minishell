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

size_t	get_command_number(t_token *tokens)
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

size_t	get_command_size(t_token *tokens)
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

void	fill_command(t_token **tokens, char ***cmd)
{
	size_t	i;

	i = 0;
	*cmd = ft_calloc(get_command_size(*tokens) + 1, sizeof(char *));
	while (*tokens && (*tokens)->type != PIPE)
	{
		if ((*tokens)->type == WORD)
			(*cmd)[i] = (*tokens)->content;
		*tokens = (*tokens)->next;
		i++;
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

void	close_fds(int *fd, int file)
{
	if (file != -1)
		close(file);
	close(fd[0]);
	close(fd[1]);
}

bool	check_folder(char *path)
{
	int	fd;

	fd = open(path, O_WRONLY);
	if (errno == 21)
	{
		if (fd != -1)
			close(fd);
		return (true);
	}
	if (fd != -1)
		close(fd);
	return (false);
}

void	exec_fullpath(char **args, char **envp)
{
	int	temp;

	execve(args[0], args, envp);
	temp = errno;
	if (check_folder(args[0]))
		(handle_error(args[0], 21), ft_arrayclear(args), exit(127));
	if (temp == 2)
		(handle_error(args[0], -1), ft_arrayclear(args), exit(127));
	else if (temp != 8)
		handle_error(args[0], temp);
	ft_arrayclear(args);
	exit(126);
}

void	exec(char **cmd, t_list *envp_lst)
{
	char **envp;

	// printf("%s\n", cmd[0]);
	if (!cmd[0])
		(handle_error("''", -1), exit(127));
	envp = env_lst_to_str(envp_lst);
	if (cmd[0][0] == '/')
		exec_fullpath(cmd, envp);
	// if (check_path_cmd(cmd[0], envp) == 1)
	// 	(handle_error(cmd[0], -1), ft_arrayclear(cmd), exit(127));
	// else if (check_path_cmd(cmd[0], envp) == 2)
	// 	(handle_error(cmd[0], 13), ft_arrayclear(cmd), exit(127));
	// cmd[0] = get_path_cmd(cmd[0], envp);
	// execve(cmd[0], cmd, envp);
	// handle_error("Exec", errno);
	ft_arrayclear(cmd);
	// free(cmd);
	exit(127);
}

void	file_bonus(char *file, bool mode, int *fd, bool here_doc)
{
	int	file_fd;

	if (mode && !here_doc)
		file_fd = open(file, O_RDONLY);
	else if (!mode && !here_doc)
		file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (mode && here_doc)
		file_fd = open(".here_doc", O_RDONLY);
	else
		file_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (file_fd < 0 && mode)
		(handle_error(file, errno), close_fds(fd, -1), exit(1));
	else if (file_fd < 0 && !mode)
		(handle_error(file, errno), close_fds(fd, -1), exit(1));
	if (mode)
	{
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);
	}
	else
	{
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
}

int	pipex_bonus(char **cmd, t_list *envp, int state)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) < 0)
		(handle_error("Pipe", errno), exit(1));
	pid = fork();
	if (pid < 0)
		(handle_error("Fork", errno), exit(1));
	if (pid == 0)
	{
		if (state <= 0)
		{
			if (state == -1)
				file_bonus("/dev/stdin", true, fd, (state == -2));
			dup2(fd[1], STDOUT_FILENO);
			close_fds(fd, -1);
		}
		else
			file_bonus("/dev/stdout", false, fd, (state == 2));
		close_fds(fd, -1);
		exec(cmd, envp);
	}
	dup2(fd[0], STDIN_FILENO);
	close_fds(fd, -1);
	return (pid);
}

void	pipex(size_t nbr_cmd, char ***cmd, t_list *envp)
{
	size_t	i;

	i = 0;
	pipex_bonus(cmd[i], envp, -1);
	while (i < nbr_cmd - 1)
	{
		pipex_bonus(cmd[i], envp, 0);
		i++;
	}
	pipex_bonus(cmd[i], envp, 1);
	// 			, argv[argc - 1], state)
	// return (wait_process(pipex_bonus(argv[argc - 2], envp
	// 			, argv[argc - 1], state)));
}

void	exec_one(char **cmd, t_list *envp_lst)
{
	int		temp;
	pid_t	pid;
	char	**envp;

	pid = fork();
	if (pid < 0)
		(handle_error("Fork", errno), exit(1));
	if (pid == 0)
	{
		envp = env_lst_to_str(envp_lst);
		execve(cmd[0], cmd, envp);
		temp = errno;
		if (check_folder(cmd[0]))
			(handle_error(cmd[0], 21), ft_arrayclear(cmd), exit(127));
		if (temp == 2)
			(handle_error(cmd[0], -1), ft_arrayclear(cmd), exit(127));
		else if (temp != 8)
			handle_error(cmd[0], temp);
		ft_arrayclear(cmd);
		exit(126);
	}
	waitpid(pid, NULL, 0);
}

void	execuction(t_token *tokens, t_list *envp)
{
	size_t	i;
	size_t	nbr_cmd;
	char	***cmd;

	i = 0;
	nbr_cmd = get_command_number(tokens);
	cmd = ft_calloc(nbr_cmd + 1, sizeof(char **));
	while (tokens)
	{
		if (tokens->type == WORD)
			fill_command(&tokens, &cmd[i]);
		// print_command(cmd[i]);
		if (tokens)
			tokens = tokens->next;
		i++;
	}
	exec_one(cmd[0], envp);
	// pipex(nbr_cmd, cmd, envp);
}
