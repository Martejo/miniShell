/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:31:50 by gemartel          #+#    #+#             */
/*   Updated: 2024/05/07 09:53:51 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**env_to_char2(t_env *env)
{
	size_t	len;
	size_t	i;
	char	**env_2d;
	t_env	*tmp;

	tmp = env;
	i = 0;
	len = strlen_env(env);
	env_2d = (char **)malloc((len + 1) * sizeof(char *));
	if (!env_2d)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	add_to_garbage(env_2d, TMP);
	tmp = env;
	while (i < len)
	{
		env_2d[i] = strdup_gc(tmp->value, TMP);
		if (!env_2d[i])
			print_and_exit(MALLOC_ERR_MSG, RED, 1);
		i++;
		tmp = tmp->next;
	}
	env_2d[i] = NULL;
	return (env_2d);
}

void	exec_process(t_btree *tree_el, t_env *env, t_io fds)
{
	char		*cmdpath;
	struct stat	stats;

	cmdpath = path_handler(tree_el, env);
	if (dup2(fds.fd_in, STDIN_FILENO) == -1)
		print_and_exit(strerror(errno), RED, 1);
	if (dup2(fds.fd_out, STDOUT_FILENO) == -1)
		print_and_exit(strerror(errno), RED, 1);
	if (fds.fd_out != 1)
		close(fds.fd_out);
	if (fds.fd_in != 0)
		close(fds.fd_in);
	if (lstat(cmdpath, &stats) != -1)
	{
		if ((stats.st_mode & S_IXUSR) && (stats.st_mode & S_IFREG))
		{
			execve(cmdpath, tree_el->cmds, env_to_char2(env));
			print_and_exit(strerror(errno), RED, 1);
		}
		else
			print_path_error(tree_el->cmds[0], 126, 2);
	}
	else
		print_path_error(tree_el->cmds[0], 127, 3);
}

char	*path_handler(t_btree *tree_el, t_env *env)
{
	char	*cmdpath;
	char	*cmd;

	cmd = tree_el->cmds[0];
	cmdpath = NULL;
	if (!cmd)
		free_and_exit(1);
	else if (cmd && (cmd[0] == '.' || cmd[0] == '\\' || cmd[0] == '/'))
	{
		if (ft_strcmp(cmd, ".") == 0 && !tree_el->cmds[1])
			print_path_error(cmd, 2, 5);
		else if (ft_strcmp(cmd, "..") == 0)
			print_path_error(cmd, 127, 1);
		else
			cmdpath = cmd;
	}
	else if (cmd && cmd[0] != '\0')
		cmdpath = get_cmd_path(cmd, env);
	check_path(cmd, cmdpath);
	return (cmdpath);
}

/*remplacer lexit par waitchild*/
int	exec_bin(t_env *env, t_btree *tree_el, t_io fds)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	status = 0;
	exit_status = 0;
	pid = fork();
	if (pid == -1)
		print_and_exit("Minishell: Fork() error.\n", RED, 1);
	if (pid == 0)
		exec_process(tree_el, env, fds);
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
