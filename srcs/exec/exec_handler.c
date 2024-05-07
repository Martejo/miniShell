/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:24:02 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 09:57:53 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_handler(t_mini *mini, t_btree *tree_el, t_io fds)
{
	int		ret_status;
	bool	cmd_builtin;
	bool	is_pipe;

	ret_status = 0;
	cmd_builtin = is_builtin(tree_el->cmds[0]);
	is_pipe = cmd_is_inside_pipe(mini->b_tree, tree_el->branch);
	if (is_pipe == true && cmd_builtin == true)
		ret_status = fork_builtin(&(mini->env), tree_el, fds);
	else if (is_pipe == false && cmd_builtin == true)
		ret_status = exec_builtin(&(mini->env), tree_el, fds);
	else
		ret_status = exec_bin(mini->env, tree_el, fds);
	return (ret_status);
}
