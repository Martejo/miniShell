/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:24 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:26 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static bool	is_dollar_expansible(t_token *tkn)
{
	if (!char_is_in_str('$', tkn->content))
		return (false);
	if (tkn->prev && tkn->prev->type == HEREDOC)
		return (false);
	if (tkn->type != WORD && tkn->type != TWO_QUOTE)
		return (false);
	return (true);
}

static bool	is_wildcard_expansible(t_token *tkn)
{
	if (!char_is_in_str('*', tkn->content))
		return (false);
	if (tkn->prev && tkn->prev->type == HEREDOC)
		return (false);
	if (tkn->type != WORD)
		return (false);
	return (true);
}

void	expander(t_mini *mini, t_tknlist *tkn_lst)
{
	t_token	*curr;

	if (g_status != 0)
		return ;
	curr = tkn_lst->head;
	while (curr)
	{
		if (is_dollar_expansible(curr))
			curr = expand_dollar(mini, curr, tkn_lst);
		curr = curr->next;
	}
	curr = tkn_lst->head;
	while (curr)
	{
		if (is_wildcard_expansible(curr))
			curr = expand_wildcard(curr, tkn_lst);
		curr = curr->next;
	}
}
