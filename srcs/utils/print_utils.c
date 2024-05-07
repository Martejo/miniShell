/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:20:13 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 10:14:50 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_and_exit(char *msg, char *color, int exit_status)
{
	write(2, color, ft_strlen(color));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	write(2, RESET, ft_strlen(RESET));
	if (exit_status)
		free_and_exit(exit_status);
}

void	print_without_exit(char *msg, char *color, int exit_status)
{
	write(2, color, ft_strlen(color));
	write(2, msg, ft_strlen(msg));
	write(2, RESET, ft_strlen(RESET));
	g_status = exit_status;
}

void	print_path_error(char *arg, int exit_status, int error)
{
	ft_printf_fd(2, "Minishell: %s", arg);
	if (error == 1)
		ft_printf_fd(2, ": command not found\n");
	if (error == 2)
		ft_printf_fd(2, ": Permission denied\n");
	if (error == 3)
		ft_printf_fd(2, ": No such file or directory\n");
	if (error == 4)
		ft_printf_fd(2, ": Is a directory\n");
	if (error == 5)
		ft_printf_fd(2, DOT_MSG_ERR);
	free_and_exit(exit_status);
}

void	print_strerror(char *arg, char *color, int new_status)
{
	write(2, color, ft_strlen(color));
	ft_printf_fd(2, "Minishell: ");
	if (arg)
		ft_printf_fd(2, "%s: ", arg);
	ft_printf_fd(2, "%s\n", strerror(errno));
	write(2, RESET, ft_strlen(RESET));
	g_status = new_status;
}
