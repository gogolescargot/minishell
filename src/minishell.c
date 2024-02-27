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

t_list	*init_minishell(int argc, char **argv, char **envp)
{
	ignore_args(argc, argv);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (init_envp(envp));
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*lst;
	t_list	*envp_lst;

	envp_lst = init_minishell(argc, argv, envp);
	while (1)
	{
		line = readline("minishell > ");
		if (!line)
			break ;
		else
		{
			add_history(line);
			lst = lexer(line);
			if (!lst)
				continue ;
			expander(lst, envp_lst);
			execution(lst, envp_lst);
			ft_free(line);
			token_clear(&lst, ft_free);
		}
	}
	ft_lstclear(&envp_lst, ft_free);
	signal_handler(0);
}
