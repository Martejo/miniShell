/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:30:44 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/07 16:15:08 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_and_exit(int exit_code)
{
	rl_clear_history();
	clear_garbage(TMP, free);
	clear_garbage(ENV, free);
	clear_garbage(B_TREE, free);
	exit(exit_code);
}
