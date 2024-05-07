/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tkntype_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:02:54 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:02:58 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_logical_op_tkn(t_tkntype tkntype)
{
	if (tkntype == AND || tkntype == OR)
		return (1);
	return (0);
}

int	is_cmd_tkn(t_tkntype tkntype)
{
	if (tkntype == WORD || tkntype == ONE_QUOTE || tkntype == TWO_QUOTE)
		return (1);
	return (0);
}

int	is_redir_tkn(t_tkntype tkntype)
{
	if (tkntype == IN || tkntype == OUT || tkntype == APPEND)
		return (1);
	return (0);
}

int	is_operator_tkn(t_tkntype tkntype)
{
	if (tkntype == PIPE || tkntype == AND || tkntype == OR)
		return (1);
	return (0);
}
