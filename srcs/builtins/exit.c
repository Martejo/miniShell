/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:37:22 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:03:50 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_limit(int sign, long result, long digit)
{
	if (sign == 1 && (result > LONG_MAX / 10
			|| (result == LONG_MAX / 10 && digit > LONG_MAX % 10)))
		return (0);
	if (sign == -1 && (result > -(LONG_MIN / 10)
			|| (result == -(LONG_MIN / 10) && digit > -(LONG_MIN % 10))))
		return (0);
	return (1);
}

int	is_valid_long(const char *str)
{
	long	result;
	int		sign;
	long	digit;

	result = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		digit = *str - '0';
		if (check_limit(sign, result, digit) == 0)
			return (0);
		result = result * 10 + digit;
		str++;
	}
	return (1);
}

int	check_status_code(char *status_code)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	while (status_code[i])
	{
		if (status_code[i] == '-' || status_code[i] == '+')
			sign++;
		if (!(status_code[i] >= 48 && status_code[i] <= 57)
			&& (status_code[i] != '-' && status_code[i] != '+'))
			return (1);
		i++;
	}
	if (sign > 1)
		return (1);
	return (0);
}

int	process_exit(char **cmds, int *exit_status)
{
	if (check_status_code(cmds[1]) == 1 || is_valid_long(cmds[1]) == 0)
	{
		ft_printf_fd(2, "Minishell: exit: %s : numeric argument required\n",
			cmds[1]);
		*exit_status = 2;
	}
	else if (cmds[2])
	{
		ft_putstr_fd("Minishell: exit: too many arguments\n", 2);
		return (1);
	}
	else
	{
		*exit_status = ft_atoi(cmds[1]);
		if (cmds[1][0] == '-' || *exit_status > 255)
			*exit_status = *exit_status % 256;
	}
	return (0);
}

int	builtin_exit(t_mini *mini, char **cmds)
{
	int		exit_status;
	char	*tmp;

	exit_status = mini->last_gstatus;
	write(2, "exit\n", 5);
	if (cmds)
	{
		if (cmds[1] && (cmds[1][0] == '-'
			|| cmds[1][0] == '+') && cmds[1][1] == 0 && cmds[2])
		{
			tmp = strjoin_gc(cmds[1], cmds[2], TMP);
			cmds[1] = tmp;
			cmds[2] = NULL;
		}
		if (cmds[1])
		{
			if (process_exit(cmds, &exit_status) == 1)
				return (1);
		}
	}
	free_and_exit(exit_status);
	return (exit_status);
}
