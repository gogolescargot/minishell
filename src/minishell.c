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
	while (envp[i])
	{
		ft_lstadd_back(&envp_lst, ft_lstnew(ft_strdup(envp[i])));
		i++;
	}
	return (envp_lst);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*lst;
	t_list	*envp_lst;

	ignore_args(argc, argv);
	envp_lst = init_envp(envp);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell > ");
		if (!line)
			signal_handler(0);
		else
		{
			add_history(line);
			lst = lexer(line);
			expander(lst, envp_lst);
			while (lst)
			{
				if (lst->content)
					printf("%s\n", lst->content);
				lst = lst->next;
			}
			free(line);
		}
	}
	ft_lstclear(&envp_lst, ft_free);
}
