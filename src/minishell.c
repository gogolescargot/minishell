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

int	main(void)
{
	char	*line;
	t_token	*lst;

	while (1)
	{
		line = readline("minishell > ");
		if (!line)
			break ;
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
