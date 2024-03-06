/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:45:07 by ggalon            #+#    #+#             */
/*   Updated: 2024/02/26 10:45:07 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	get_cmd_nbr(t_token *tokens)
{
	size_t	i;
	bool	trigger;

	i = 0;
	trigger = false;
	while (tokens)
	{
		if (tokens->type == WORD && !trigger)
		{
			i++;
			trigger = true;
		}
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

void	ft_close(int fd)
{
	if (fd > 2)
		close(fd);
}

void	ft_close_unlink(int fd, char *file)
{
	ft_close(fd);
	unlink(file);
}

int	handle_error(char *str, int error_code)
{
	g_exit_code = error_code;
	perror(str);
	return (error_code);
}
