/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 16:04:48 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:04:57 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clear_loop(void)
{
	clear_garbage(TMP, free);
	clear_garbage(TKN_LIST, free);
	clear_garbage(B_TREE, free);
}

void	clear_ctrl_c(int *stdin_cpy, char **line_read)
{
	if (*line_read)
		free(*line_read);
	*line_read = NULL;
	dup2(*stdin_cpy, STDIN_FILENO);
	close(*stdin_cpy);
	clear_loop();
}

void	sig_handler(int sigcode)
{
	if (sigcode == SIGINT)
	{
		close(STDIN_FILENO);
		write(2, "\n", 1);
		g_status = 130;
	}
	if (sigcode == SIGQUIT)
		write(2, "\b\b  \033[2D", 8);
	if (sigcode == SIGTSTP)
		write(2, "\b\b  \033[2D", 8);
}

void	process_shell(t_mini *mini, char *line_read, int *stdin_cpy)
{
	add_history(line_read);
	mini->tkn_lst = lexer(line_read);
	mini->b_tree = parser(mini);
	browse_tree(mini, mini->b_tree, mini->io_global);
	dup2(*stdin_cpy, STDIN_FILENO);
	close(*stdin_cpy);
	clear_loop();
}

void	prompt_loop(t_mini *mini)
{
	int		stdin_cpy;
	char	*line_read;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	signal(SIGTSTP, sig_handler);
	while (1)
	{
		stdin_cpy = dup(STDIN_FILENO);
		mini->last_gstatus = g_status;
		g_status = 0;
		line_read = readline(create_prompt(mini));
		if (g_status == 130)
		{
			clear_ctrl_c(&stdin_cpy, &line_read);
			continue ;
		}
		if (!line_read)
			builtin_exit(mini, NULL);
		if (line_read && *line_read)
			process_shell(mini, line_read, &stdin_cpy);
		free(line_read);
	}
}
