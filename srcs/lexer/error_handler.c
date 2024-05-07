/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 12:31:47 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/06 10:01:30 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	error_handler_lexer(int id_gc, char *msg)
{
	clear_garbage(id_gc, free);
	ft_putstr_fd(msg, 2);
	g_status = 2;
	return (-1);
}
