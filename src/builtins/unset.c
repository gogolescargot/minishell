/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 05:41:13 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/17 05:41:13 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Pop one element from a linked list and reconnect all the pointer correctly

 * @param envp A linked list that contain the environement variables
 * @param current A pointer to the current linked list node
 * @param current A pointer to the previous linked list node
 */

void	unset_pop(t_list *envp, t_list *current, t_list *prev)
{
	if (prev)
	{
		prev->next = current->next;
		free(current->content);
		free(current);
		current = prev->next;
	}
	else
	{
		envp = current->next;
		free(current->content);
		free(current);
		current = envp;
	}
}

/*
 * Utils function of unset
 *
 * @param envp A linked list that contain the environement variables
 * @param current A pointer to the current linked list node
 * @param current A pointer to the previous linked list node
 */

void	unset_utils(t_list *envp, t_list *current, t_list *prev, char *cmd)
{
	while (current)
	{
		if (is_env(current->content, cmd))
		{
			unset_pop(envp, current, prev);
			break ;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

/*
 * Unset one or multiple environment variables
 *
 * @param cmd An array of strings containing the command itself and the arguments
 * @param envp A linked list that contain the environement variables
 * @return 0 on success, 1 in case of error
 */

int	ft_unset(char **cmd, t_list *envp)
{
	t_list	*current;
	t_list	*prev;
	size_t	i;

	i = 1;
	if (!cmd)
		return (1);
	while (cmd[i])
	{
		prev = NULL;
		current = envp;
		unset_utils(envp, current, prev, cmd[i]);
		i++;
	}
	return (0);
}
