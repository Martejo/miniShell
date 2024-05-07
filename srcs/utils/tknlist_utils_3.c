/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tknlist_utils_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:02:45 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:02:47 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unbound_token_in_place(t_tknlist *list_tkn, t_token *to_pop)
{
	if (to_pop == NULL)
		return ;
	if (to_pop != list_tkn->head)
		(to_pop->prev)->next = to_pop->next;
	else
		list_tkn->head = to_pop->next;
	if (to_pop != list_tkn->tail)
		(to_pop->next)->prev = to_pop->prev;
	else
		list_tkn->tail = to_pop->prev;
}

void	tknlst_addfront(t_token *curr, t_tknlist *tknlst)
{
	if (!curr || !tknlst)
		return ;
	curr->prev = NULL;
	curr->next = tknlst->head;
	if (tknlst->head)
		tknlst->head->prev = curr;
	if (!tknlst->tail)
		tknlst->tail = curr;
	tknlst->head = curr;
}

void	tknlst_addback(t_token *curr, t_tknlist *tknlst)
{
	if (!curr || !tknlst)
		return ;
	curr->next = NULL;
	if (!tknlst->head)
	{
		tknlst->head = curr;
		tknlst->tail = curr;
	}
	else
	{
		curr->prev = tknlst->tail;
		tknlst->tail->next = curr;
		tknlst->tail = curr;
	}
}
