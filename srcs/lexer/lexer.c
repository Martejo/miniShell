/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:12:00 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:04:20 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	detect_type(const char c, const char c2)
{
	if (c == '\'')
		return (ONE_QUOTE);
	else if (c == '\"')
		return (TWO_QUOTE);
	else if (c == '(')
		return (PARENTHESIS);
	else if (c == '|' && c2 != '|')
		return (PIPE);
	else if (c == '<' && c2 != '<')
		return (IN);
	else if (c == '>' && c2 != '>')
		return (OUT);
	else if (c == '>' && c2 == '>')
		return (APPEND);
	else if (c == '<' && c2 == '<')
		return (HEREDOC);
	else if (c == '&' && c2 == '&')
		return (AND);
	else if (c == '|' && c2 == '|')
		return (OR);
	else if (c == '&' && c2 != '&')
		print_without_exit(\
			"Minishell: syntax error near unexpected token `&'\n", RED, 2);
	return (WORD);
}

void	handle_token(char *buffer, t_tknlist *list, t_tkntype type, int *i)
{
	if (type == TWO_QUOTE)
		*i += double_quote_handler(buffer, list);
	else if (type == ONE_QUOTE)
		*i += simple_quote_handler(buffer, list);
	else if (type == PIPE)
		*i += pipe_handler(buffer, list);
	else if (type == AND || type == OR)
		*i += operator_handler(buffer, list, type);
	else if (type == HEREDOC || type == IN || type == OUT || type == APPEND)
		*i += file_handler(buffer, list, type);
	else if (type == PARENTHESIS)
		*i += parenthese_handler(buffer, list);
	else if (type == WORD)
		*i += cmd_handler(buffer, list);
}

int	is_only_space(char *buffer)
{
	size_t	i;

	i = 0;
	if (!buffer)
		return (1);
	while (buffer[i])
	{
		if (!ft_isspace(buffer[i]))
			return (0);
		i++;
	}
	return (1);
}

t_tknlist	*lexer(char *buffer)
{
	int			i;
	t_tkntype	type;
	t_tknlist	*list;

	if (!buffer || g_status != 0 || is_only_space(buffer) == 1)
		return (NULL);
	init_list(&list);
	i = 0;
	while (buffer[i])
	{
		if (!ft_isspace(buffer[i]))
		{
			if (detect_error_type(buffer[i]) == -1)
				break ;
			type = detect_type(buffer[i], buffer[i + 1]);
			handle_token(&buffer[i], list, type, &i);
			if (g_status != 0)
				break ;
		}
		else
			i++;
	}
	return (list);
}
