/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:47:33 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 10:10:22 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_error_export(char *args, int error)
{
	if (error == 1)
		ft_printf_fd(2, "%sMinishell: export: `%s': not a valid identifier\n%s",
			RED, args, RESET);
	return (1);
}

int	is_valid_args(const char *args)
{
	int	i;

	i = 0;
	if (ft_isdigit(args[i]) == 1 || args[i] == '=')
		return (print_error_export((char *)args, 1));
	while (args[i] && args[i] != '=')
	{
		if (ft_isalnum(args[i]) == 0 && args[i] != '_')
			return (print_error_export((char *)args, 1));
		i++;
	}
	if (args[i] != '=')
		return (2);
	return (0);
}

int	env_add(char *value, t_env **env, int mod)
{
	t_env	*new;

	new = NULL;
	new = malloc_gc(sizeof(t_env), ENV);
	if (!new)
		free_and_exit(1);
	new->value = ft_strndup(value, ft_strlen(value), ENV);
	new->secret = mod;
	new->next = NULL;
	env_add_back(env, new);
	return (0);
}

int	export_handler(char *args, t_env **env)
{
	int	new_env;
	int	error_ret;

	new_env = 0;
	error_ret = 0;
	error_ret = is_valid_args(args);
	if (error_ret == 1)
		return (1);
	new_env = is_in_env((*env), args);
	if (new_env == 0)
	{
		if (error_ret == 2)
			env_add(args, env, 1);
		else
			env_add(args, env, 0);
	}
	return (0);
}

int	ft_export(char **args, t_env **envt, t_io fds)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		print_sorted_env(*envt, fds);
		return (0);
	}
	else
	{
		while (args[i] != NULL)
		{
			if (args[i][0] == 0)
			{
				print_error_export("", 1);
				return (1);
			}
			if (export_handler(args[i], envt) != 0)
				return (1);
			i++;
		}
	}
	return (0);
}
