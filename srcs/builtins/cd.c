/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:59:31 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 14:59:33 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_oldpwd(t_env **env)
{
	char	cwd[PATH_MAX];
	char	*oldpwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	oldpwd = strjoin_gc("OLDPWD=", cwd, TMP);
	if (!oldpwd)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	if (is_in_env(*env, oldpwd) == 0)
		env_add(oldpwd, env, 0);
	return (0);
}

int	update_pwd(t_env **env, int slash)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	if (!slash)
		pwd = strjoin_gc("PWD=", cwd, TMP);
	else
		pwd = strjoin_gc("PWD=", "//", TMP);
	if (!pwd)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	if (is_in_env(*env, pwd) == 0)
		env_add(pwd, env, 0);
	return (0);
}

int	go_to_path(t_env **env)
{
	int		ret;
	char	*env_path;

	env_path = NULL;
	update_oldpwd(env);
	env_path = get_env_value(*env, "HOME=", 4);
	if (!env_path)
		free_and_exit(1);
	else if (ft_strcmp(env_path, "") == 0)
	{
		ft_putstr_fd("Minishell: cd: HOME not set\n", 2);
		return (1);
	}
	ret = chdir(env_path);
	if (ret)
	{
		ft_putstr_fd(strerror(errno), 2);
		return (1);
	}
	update_pwd(env, 0);
	return (ret);
}

int	process_cd(char **cmds, t_env **env)
{
	int	ret_cd;
	int	mod;

	mod = 0;
	if (update_oldpwd(env) != 0)
		return (1);
	ret_cd = chdir(cmds[1]);
	if (ret_cd < 0)
	{
		ft_printf_fd(2, "Minishell: cd: %s: %s\n", cmds[1], strerror(errno));
		return (1);
	}
	if (ft_strcmp(cmds[1], "//") == 0)
		mod = 1;
	if (update_pwd(env, mod) != 0)
		return (1);
	return (ret_cd);
}

int	cd(char **cmds, t_env **env)
{
	if (cmds[1] == NULL)
		return (go_to_path(env));
	else if (cmds[2])
	{
		ft_printf_fd(2, "Minishell: cd: too many arguments\n");
		return (1);
	}
	return (process_cd(cmds, env));
}
