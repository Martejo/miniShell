/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler_suite.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:20:40 by gemartel          #+#    #+#             */
/*   Updated: 2024/02/29 13:08:25 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parenthese_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	depth;

	i = 0;
	depth = 0;
	while (buffer[i])
	{
		if (buffer[i] == '(')
			depth++;
		else if (buffer[i] == ')')
		{
			depth--;
			if (depth == 0)
			{
				if (!add_node(list,
						create_node(PARENTHESIS,
							ft_strndup(buffer, i + 1, TKN_LIST), 0)))
					print_and_exit(MALLOC_ERR_MSG, RED, 1);
				return (i + 1);
			}
		}
		i++;
	}
	return (error_handler_lexer(TKN_LIST, PARENTHESIS_ERR_MSG));
}

int	double_quote_handler(char *buffer, t_tknlist *list)
{
	int	i;
	int	link;

	i = 1;
	link = 0;
	while (buffer[i])
	{
		if (buffer[i] == '\"')
			break ;
		i++;
	}
	if (buffer[i] == '\0')
		return (error_handler_lexer(TKN_LIST, QUOTES_ERR_MSG));
	if (ft_isspace(buffer[i + 1]) == 0)
		link = 1;
	if (!add_node(list,
			create_node(TWO_QUOTE, ft_strndup(buffer, i + 1, TKN_LIST), link)))
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	return (i + 1);
}

int	operator_handler(char *buffer, t_tknlist *list, t_tkntype type)
{
	if (!add_node(list, create_node(type, ft_strndup(buffer, 2, TKN_LIST), 0)))
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	return (2);
}
