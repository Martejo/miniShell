/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:57 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:59 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_token_index(t_token	*curr)
{
	size_t	i;

	i = 0;
	while (curr)
	{
		curr->index = i;
		if (is_redir_tkn(curr->type) || curr->type == HEREDOC)
			curr->priority = 1;
		if (curr->type == PIPE)
			curr->priority = 3;
		else if (is_logical_op_tkn(curr->type))
			curr->priority = 4;
		i++;
		curr = curr->next;
	}
}

t_btree	*parser(t_mini *mini)
{
	if (g_status != 0 || !mini->tkn_lst)
	{
		clear_loop();
		return (NULL);
	}
	verify_syntax_tknlist(mini->tkn_lst);
	if (g_status != 0)
	{
		clear_loop();
		return (NULL);
	}
	reducer(mini->tkn_lst->head);
	expander(mini, mini->tkn_lst);
	linker(mini->tkn_lst);
	rearrange_cmd_redir_order(mini->tkn_lst);
	set_token_index(mini->tkn_lst->head);
	return (create_bin_tree(mini->tkn_lst));
}
