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

int	init_envp(char **envp, t_list **envp_lst)
{
	size_t	i;
	t_list	*node;

	i = 0;
	*envp_lst = NULL;
	while (envp[i])
	{
		node = ft_lstnew(ft_strdup(envp[i]));
		if (!node)
			return (ft_lstclear(&*envp_lst, ft_free), 1);
		ft_lstadd_back(&*envp_lst, node);
		i++;
	}
	return (0);
}

t_data	*init_minishell(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->envp = envp;
	init_envp(envp, &data->envp_lst);
	data->tokens = NULL;
	data->cmd = NULL;
	(void)argc;
	(void)argv;
	return (data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = init_minishell(argc, argv, envp);
	if (!data)
		return (1);
	while (1)
	{
		signal(SIGINT, signal_handler);
		signal(SIGQUIT, SIG_IGN);
		data->line = readline("minishell > ");
		if (!data->line)
			break ;
		add_history(data->line);
		lexer(data);
		if (!data->tokens)
			continue ;
		expander(data);
		execution(data);
		ft_free(data->line);
		token_clear(&data->tokens, ft_free);
	}
	ft_lstclear(&data->envp_lst, ft_free);
	printf("\nexit\n");
	secure_exit(&data, g_exit_code);
}
