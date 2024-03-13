/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:41:23 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:41:23 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Display all environment variables
 *
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_env(t_list *envp)
{
	if (!envp)
		return (0);
	while (envp)
	{
		if (printf("%s\n", (char *)envp->content) == -1)
			return (1);
		envp = envp->next;
	}
	return (0);
}
