/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 11:14:00 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 12:37:06 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env(t_env *env, t_io fds)
{
	while (env)
	{
		if (env->secret == 0 && env->value != NULL)
		{
			ft_putstr_fd(env->value, fds.fd_out);
			write(fds.fd_out, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}
