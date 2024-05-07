/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 14:40:47 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 09:55:24 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	size_cmds(char **cmds)
{
	int	size;

	size = 0;
	while (cmds[size])
		size++;
	return (size);
}

static int	is_valid_option(char *option)
{
	int	i;

	i = 1;
	if (option[1] == '\0')
		return (1);
	while (option[i])
	{
		if (option[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	echo(char **cmds, t_io fds)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	if (size_cmds(cmds) > 1)
	{
		while (cmds[i] && is_valid_option(cmds[i]) == 0)
		{
			if (n_option == 0)
				n_option = 1;
			i++;
		}
		while (cmds[i])
		{
			ft_putstr_fd(cmds[i], fds.fd_out);
			if (cmds[i + 1])
				write(fds.fd_out, " ", 1);
			i++;
		}
	}
	if (n_option == 0)
		write(fds.fd_out, "\n", 1);
	return (0);
}
