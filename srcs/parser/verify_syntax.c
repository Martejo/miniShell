/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:02:05 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:02:06 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_empty_parenthis(char *content)
{
	size_t	i;

	i = 1;
	while (content[i] && content[i] != ')')
	{
		if (ft_isspace(content[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

static int	verify_syntax_head(t_token *head)
{
	if (is_operator_tkn(head->type))
	{
		ft_printf_fd(2, "%s%s `%s'%s\n", \
			RED, NEAR_TOKEN_ERR_MSG, head->content, RESET);
		return (2);
	}
	if (head->type == PARENTHESIS && is_empty_parenthis(head->content))
	{
		ft_printf_fd(2, "%s%s `%c'%s\n", RED, NEAR_TOKEN_ERR_MSG, ')', RESET);
		return (2);
	}
	return (0);
}

static int	verify_syntax_tail(t_token *tail)
{
	if (tail->type == PIPE)
	{
		ft_printf_fd(2, "%s%s%s\n", RED, OPEN_PIPE_ERR_MSG, RESET);
		return (2);
	}
	if (tail->type == AND || tail->type == OR)
	{
		ft_printf_fd(2, "%s%s%s\n", RED, OPEN_LOGICAL_OP_ERR_MSG, RESET);
		return (2);
	}
	if (is_redir_tkn(tail->type) || tail->type == HEREDOC)
	{
		ft_printf_fd(2, "%s%s `%s'%s\n", \
			RED, NEAR_TOKEN_ERR_MSG, "newline", RESET);
		return (2);
	}
	if (tail->type == PARENTHESIS && is_empty_parenthis(tail->content))
	{
		ft_printf_fd(2, "%s%s `%c'%s\n", RED, NEAR_TOKEN_ERR_MSG, '(', RESET);
		return (2);
	}
	return (0);
}

static int	verify_syntax_body(t_token *curr)
{
	while (curr && curr->next && g_status == 0)
	{
		if ((is_operator_tkn(curr->type) \
			|| is_redir_tkn(curr->type)) && is_operator_tkn(curr->next->type))
		{
			ft_printf_fd(2, "%s%s `%s'%s\n", \
				RED, NEAR_TOKEN_ERR_MSG, curr->next->content, RESET);
			return (2);
		}
		if (is_redir_tkn(curr->type) && curr->next->type == WORD \
			&& char_is_in_str('*', curr->next->content))
		{
			ft_printf_fd(2, "%s%s%s", RED, STAR_TOKEN_ERR_MSG, RESET);
			return (1);
		}
		if (curr->type == PARENTHESIS && is_empty_parenthis(curr->content) == 1)
		{
			ft_printf_fd(2, "%s%s `%c'%s\n", \
				RED, NEAR_TOKEN_ERR_MSG, '(', RESET);
			return (2);
		}
		curr = curr->next;
	}
	return (0);
}

void	verify_syntax_tknlist(t_tknlist *lst)
{
	int	exit_status;

	exit_status = verify_syntax_head(lst->head);
	if (exit_status != 0)
	{
		g_status = exit_status;
		return ;
	}
	exit_status = verify_syntax_body(lst->head);
	if (exit_status != 0)
	{
		g_status = exit_status;
		return ;
	}
	g_status = verify_syntax_tail(lst->tail);
}
