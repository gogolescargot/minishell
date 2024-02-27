/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 17:16:44 by ggalon            #+#    #+#             */
/*   Updated: 2024/01/27 17:16:44 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_exit_code = 0;

void	secure_exit(t_data **data, int error_code)
{
	ft_free((*data)->line);
	ft_lstclear(&(*data)->envp_lst, ft_free);
	token_clear(&(*data)->tokens, ft_free);
	commands_clear(&(*data)->cmd);
	unlink(".here_doc");
	free(*data);
	exit(error_code);
}

void	ignore_args(int argc, char **argv)
{
	(void)argc;
	(void)argv;
}

t_list	*init_envp(char **envp)
{
	size_t	i;
	t_list	*envp_lst;

	i = 0;
	envp_lst = NULL;
	while (envp[i])
	{
		ft_lstadd_back(&envp_lst, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
	return (envp_lst);
}

t_data	*init_minishell(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	data->envp = envp;
	data->envp_lst = init_envp(envp);
	data->tokens = NULL;
	data->cmd = NULL;
	ignore_args(argc, argv);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = init_minishell(argc, argv, envp);
	while (1)
	{
		data->line = readline("minishell > ");
		if (!data->line)
			break ;
		else
		{
			add_history(data->line);
			data->tokens = lexer(data->line);
			if (!data->tokens)
				continue ;
			expander(data);
			execution(data);
			ft_free(data->line);
			token_clear(&data->tokens, ft_free);
		}
	}
	ft_lstclear(&data->envp_lst, ft_free);
	printf("exit\n");
	secure_exit(&data, g_exit_code);
}
