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
		if ((*tokens)->type == WORD && is_builtin((*tokens)->content) == BUILTIN_NONE && i == 0)
			(*cmd)[i] = get_cmd_path((*tokens)->content, envp_lst);
		else if ((*tokens)->type == WORD)
		{
			(*cmd)[i] = ft_strdup((*tokens)->content);
		}
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

int	exec_bin(char *cmd, char **args, char **envp)
{
	execve(cmd, args, envp);
	free(envp);
	ft_arrayclear(args);
	return (1);
}


int	wait_process(int pid)
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


int	pipeline(char ***cmd, t_list *envp_lst)
{
	int		fd[2];
	int		fdd = 0;
	char	**envp;
	enum e_builtin	type;
	pid_t	pid;

	envp = env_lst_to_str(envp_lst);
	while (*cmd)
	{
		type = is_builtin(*cmd[0]);
		if (type != BUILTIN_NONE)
		{
			exec_builtin(*cmd, envp_lst, type);
			return (0);
		}
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		else if (pid == 0)
		{
			dup2(fdd, 0);
			if (*(cmd + 1) != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			exec_bin((*cmd)[0], *cmd, envp);
			exit(1);
		}
		else
		{
			// close(fd[0]);
			close(fd[1]);
			fdd = fd[0];
			cmd++;
		}
	}
	free(envp);
	return (pid);
}

void execute(char ***cmd, char *infile, char *outfile, t_list *envp_lst)
{
	//save in/out
	int	tmpin = dup(0);
	int	tmpout = dup(1);
	char **envp = env_lst_to_str(envp_lst);

	//set the initial input
	int	fdin;
	if (infile)
		fdin = open(infile, O_RDONLY);
	else
	// Use default input
		fdin = dup(tmpin);
	int	ret;
	int	fdout;
	while (*cmd)
	{
		//redirect input
		dup2(fdin, 0);
		close(fdin);
		//setup output
		if (*(cmd + 1) == NULL)
		{
			// Last simple command
			if (outfile)
				fdout = open(outfile, O_WRONLY);
			else
				// Use default output
				fdout = dup(tmpout);
		}
		else
		{
			// Not last
			//simple command
			//create pipe
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}// if/else

		// Redirect output
		dup2(fdout,1);
		close(fdout);

		// Create child process
		enum e_builtin type = is_builtin(*(cmd)[0]);
		if (type != BUILTIN_NONE)
		{
			exec_builtin(*cmd, envp_lst, type);
			return ;
		}
		ret = fork();
		if (ret == 0)
		{
			execve(*(cmd)[0], *cmd, envp);
			perror("fork");
			exit(1);
		}
		cmd++;
	} // for

	//restore in/out defaults
	dup2(tmpin,0);
	dup2(tmpout,1);
	close(tmpin);
	close(tmpout);
	ft_free(envp);

	// Wait for last command
	waitpid(ret, NULL, 0);

} // execute

void	execution(t_token *tokens, t_list *envp_lst)
{
	size_t	i;
	size_t	nbr_cmd;
	char	***cmd;

	if (!tokens)
		return ;
	i = 0;
	nbr_cmd = get_cmd_nbr(tokens);
	cmd = ft_calloc(nbr_cmd + 1, sizeof(char **));
	while (tokens)
	{
		if (tokens->type == WORD)
			fill_command(&tokens, &cmd[i], envp_lst);
		if (tokens)
			tokens = tokens->next;
		i++;
	}
	execute(cmd, NULL, NULL, envp_lst);
	// int pid = pipeline(cmd, envp_lst);
	// wait_process(pid);
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
