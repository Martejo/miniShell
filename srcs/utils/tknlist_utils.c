/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:15:16 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/07 13:08:21 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*create_node(t_tkntype typed, char *value, int linked)
{
	t_token	*node;

	if (!value)
		return (NULL);
	node = malloc_gc(sizeof(t_token), TKN_LIST);
	if (!node)
		return (NULL);
	node->content = value;
	node->type = typed;
	node->link = linked;
	node->index = 0;
	node->priority = 0;
	node->used_flag = 0;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

int	add_node(t_tknlist *list, t_token *node)
{
	if (!node)
		return (-1);
	if (!list->head)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	return (1);
}

void	init_list(t_tknlist **list)
{
	(*list) = malloc_gc(sizeof(t_tknlist), TKN_LIST);
	if (!(*list))
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	(*list)->head = NULL;
	(*list)->tail = NULL;
}
