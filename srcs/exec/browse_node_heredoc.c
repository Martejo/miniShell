/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   browse_node_heredoc.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:59:56 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 14:59:58 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_signals_heredoc(char *input, size_t	nb_lines, char *delim)
{
	if (!input)
	{
		if (g_status == 130)
			write(2, "\n", 1);
		else
			ft_printf_fd(2, HEREDOC_MSG, RED, (int) nb_lines, delim, RESET);
	}
}

int	readline_heredoc(t_btree *tree_el, char *line \
	, size_t *nb_lines, int fd_pipe[2])
{
	if (ft_strlen(line) > 0 && ft_strcmp(line, tree_el->cmds[0]) == 0)
	{
		free(line);
		return (1);
	}
	ft_printf_fd(fd_pipe[FD_WRITE], "%s\n", line);
	free(line);
	(*nb_lines)++;
	return (0);
}

void	traverse_heredoc_node(t_mini *mini, t_btree *tree_el, t_io io_inherited)
{
	int		fd_pipe[2];
	char	*line;
	t_io	io_transmitted;
	size_t	nb_lines;

	ft_memcpy(&io_transmitted, &io_inherited, sizeof(t_io));
	if (pipe(fd_pipe) == -1)
		print_and_exit("Minishell: pipe error\n", RED, 1);
	line = readline("> ");
	nb_lines = 1;
	while (line)
	{
		if (readline_heredoc(tree_el, line, &nb_lines, fd_pipe) == 1)
			break ;
		line = readline("> ");
	}
	handle_signals_heredoc(line, nb_lines, tree_el->cmds[0]);
	close(fd_pipe[FD_WRITE]);
	if (io_inherited.fd_in != 0)
		close(io_inherited.fd_in);
	io_transmitted.fd_in = fd_pipe[FD_READ];
	browse_tree(mini, tree_el->left, io_transmitted);
	close(fd_pipe[FD_READ]);
}
