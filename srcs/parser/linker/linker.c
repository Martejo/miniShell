/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:30 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:32 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_link_sensitive(t_tkntype type)
{
	if (type == WORD || type == ONE_QUOTE || type == TWO_QUOTE \
		|| type == APPEND || type == HEREDOC || type == IN || type == OUT)
		return (1);
	return (0);
}

void	linker(t_tknlist *tkn_lst)
{
	t_token	*curr;
	t_token	*next;
	char	*new_content;

	curr = tkn_lst->head;
	while (curr)
	{
		next = curr->next;
		if (next && curr->link == 1 \
			&& is_link_sensitive(curr->type) && is_link_sensitive(next->type))
		{
			new_content = strjoin_gc(curr->content, next->content, TKN_LIST);
			if (!new_content)
				print_and_exit(MALLOC_ERR_MSG, RED, 1);
			curr->link = next->link;
			del_one_garbage(curr->content, TKN_LIST);
			pop_token_in_place(tkn_lst, next);
			curr->content = new_content;
			continue ;
		}
		curr = curr->next;
	}
}
