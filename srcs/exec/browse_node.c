/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:00:02 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:00:04 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	traverse_redir_input_node(t_mini *mini, t_btree *tree_el \
	, t_io io_inherited)
{
	t_io	io_transmitted;

	if (io_inherited.fd_in != 0)
		close(io_inherited.fd_in);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	io_transmitted.fd_in = open(tree_el->cmds[0], O_RDONLY);
	if (io_transmitted.fd_in == -1)
	{
		print_strerror(tree_el->cmds[0], RED, 1);
		return ;
	}
	browse_tree(mini, tree_el->left, io_transmitted);
	if (io_transmitted.fd_in != 0)
		close(io_transmitted.fd_in);
}

void	traverse_redir_output_node(t_mini *mini, t_btree *tree_el \
	, t_io io_inherited)
{
	t_io	io_transmitted;

	if (io_inherited.fd_out != 0 && io_inherited.fd_out != 1)
		close(io_inherited.fd_out);
	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if (tree_el->type == OUT)
		io_transmitted.fd_out = open(tree_el->cmds[0] \
			, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		io_transmitted.fd_out = open(tree_el->cmds[0] \
			, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (io_transmitted.fd_out == -1)
	{
		print_strerror(tree_el->cmds[0], RED, 1);
		return ;
	}
	browse_tree(mini, tree_el->left, io_transmitted);
	if (io_transmitted.fd_out != 0 && io_transmitted.fd_out != 1)
		close(io_transmitted.fd_out);
}

void	traverse_pipe_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	int		fd_pipe[2];
	t_io	io_transmitted;

	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if (pipe(fd_pipe) == -1)
	{
		print_strerror("pipe", RED, 1);
		return ;
	}
	io_transmitted.fd_out = fd_pipe[FD_WRITE];
	browse_tree(mini, tree_el->left, io_transmitted);
	close(fd_pipe[FD_WRITE]);
	io_transmitted.fd_in = fd_pipe[FD_READ];
	io_transmitted.fd_out = io_inherited.fd_out;
	browse_tree(mini, tree_el->right, io_transmitted);
	close(fd_pipe[FD_READ]);
}

void	traverse_logical_op_node(t_mini *mini, t_btree *tree_el \
	, t_io io_inherited)
{
	browse_tree(mini, tree_el->left, io_inherited);
	if ((tree_el->type == AND && g_status == 0)
		|| (tree_el->type == OR && g_status != 0))
		browse_tree(mini, tree_el->right, io_inherited);
}

int	traverse_parenthesis_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	pid_t	pid;
	char	*new_prompt;

	pid = fork();
	if (pid == -1)
		print_and_exit("Minishell : Fork error\n", RED, 1);
	if (pid == 0)
	{
		new_prompt = strdup_gc(tree_el->cmds[0], TMP);
		clear_garbage(B_TREE, free);
		mini->b_tree = NULL;
		mini->tkn_lst = lexer(new_prompt);
		mini->b_tree = parser(mini);
		browse_tree(mini, mini->b_tree, io_inherited);
		free_and_exit(g_status);
	}
	return (wait_child(pid));
}
