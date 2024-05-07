/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:00:09 by gemartel          #+#    #+#             */
/*   Updated: 2024/05/07 09:53:27 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	wait_child(pid_t pid)
{
	int	status;
	int	exit_status;

	exit_status = 0;
	status = 0;
	waitpid(pid, &status, 0);
	if (WCOREDUMP(status) && WTERMSIG(status) == 11)
	{
		g_status = 139;
		ft_putendl_fd("Segmentation fault (core dumped)", 2);
	}
	if (WCOREDUMP(status) && WTERMSIG(status) == 3)
		ft_putendl_fd("Quit (core dumped)", 2);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	return (exit_status);
}

void	browse_tree(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	if (!tree_el || g_status == 130)
		return ;
	if (tree_el->type == AND || tree_el->type == OR)
		traverse_logical_op_node(mini, tree_el, io_inherited);
	else if (tree_el->type == PIPE)
		traverse_pipe_node(mini, tree_el, io_inherited);
	else if (tree_el->type == IN)
		traverse_redir_input_node(mini, tree_el, io_inherited);
	else if (tree_el->type == HEREDOC)
		traverse_heredoc_node(mini, tree_el, io_inherited);
	else if (tree_el->type == OUT || tree_el->type == APPEND)
		traverse_redir_output_node(mini, tree_el, io_inherited);
	else if (tree_el->type == PARENTHESIS)
		g_status = traverse_parenthesis_node(mini, tree_el, io_inherited);
	else if (tree_el->type == WORD)
		g_status = exec_handler(mini, tree_el, io_inherited);
}
