/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:04:25 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:16:26 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_status = 0;

t_mini	*singleton_mini(t_mini *address_mini)
{
	static t_mini	*mini_todeliver;

	if (address_mini)
		mini_todeliver = address_mini;
	return (mini_todeliver);
}

void	handle_shlvl(t_env *env, int nb)
{
	int		lvl;
	char	*shlvl;

	lvl = ft_atoi(get_env_value(env, "SHLVL", 5));
	if (lvl >= 999)
	{
		ft_printf_fd(2, SHLVL_ERR_MSG, lvl + 1);
		lvl = 1;
	}
	else if (lvl < 0)
		lvl = 0;
	else
		lvl += nb;
	shlvl = itoa_gc(lvl, ENV);
	if (!shlvl)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	shlvl = strjoin_gc("SHLVL=", shlvl, TMP);
	if (!shlvl)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	is_in_env(env, shlvl);
	rl_outstream = stderr;
}

int	main(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void) argv;
	if (argc > 1)
		print_and_exit(ARGS_ERR_MSG, RED, 1);
	if (!envp[0])
		print_and_exit(UNAVAILABLE_ENV, RED, 1);
	mini.env = init_env(envp);
	mini.io_global = (t_io){0, 1};
	mini.last_gstatus = 0;
	singleton_mini(&mini);
	handle_shlvl(mini.env, 1);
	prompt_loop(&mini);
	return (0);
}
