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

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*lst;

	(void)argc;
	(void)argv;
	(void)envp;
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
			expander(lst);
			while (lst)
			{
				if (lst->content)
					printf("%s\n", lst->content);
				lst = lst->next;
			}
			free(line);
		}
	}
}
