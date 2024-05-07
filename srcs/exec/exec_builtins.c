/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:47:27 by gemartel          #+#    #+#             */
/*   Updated: 2024/05/07 09:54:11 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	else if (ft_strcmp(command, "cd") == 0)
		return (1);
	else if (ft_strcmp(command, "pwd") == 0)
		return (1);
	else if (ft_strcmp(command, "env") == 0)
		return (1);
	else if (ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strcmp(command, "unset") == 0)
		return (1);
	else if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin( t_env **envt, t_btree *tree_el, t_io fds)
{
	int	status;

	status = 0;
	if (ft_strcmp(tree_el->cmds[0], "echo") == 0)
		status = echo(tree_el->cmds, fds);
	else if (ft_strcmp(tree_el->cmds[0], "cd") == 0)
		status = cd(tree_el->cmds, envt);
	else if (ft_strcmp(tree_el->cmds[0], "pwd") == 0)
		status = ft_pwd(fds, *envt);
	else if (ft_strcmp(tree_el->cmds[0], "env") == 0)
		env(*envt, fds);
	else if (ft_strcmp(tree_el->cmds[0], "export") == 0)
		status = ft_export(tree_el->cmds, envt, fds);
	else if (ft_strcmp(tree_el->cmds[0], "unset") == 0)
		status = unset(tree_el->cmds, envt);
	else if (ft_strcmp(tree_el->cmds[0], "exit") == 0)
		status = builtin_exit(singleton_mini(NULL), tree_el->cmds);
	return (status);
}

int	fork_builtin(t_env **envt, t_btree *tree_el, t_io fds)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	exit_status = 0;
	status = 0;
	pid = fork();
	if (pid == -1)
		print_and_exit("Minishell: Fork() error.\n", RED, 1);
	if (pid == 0)
	{
		status = exec_builtin(envt, tree_el, fds);
		free_and_exit(status);
	}
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
