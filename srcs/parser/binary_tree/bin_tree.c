/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_tree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:03 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:05 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*find_prior_token(t_token *curr)
{
	t_token	*target;

	target = curr;
	while (curr && curr->used_flag != 1)
	{
		if (curr->priority > 0 && curr->priority >= target->priority)
			target = curr;
		curr = curr->next;
	}
	return (target);
}

t_btree	*place_in_tree(t_btree *tree_el, t_btree *toplace, int index)
{
	if (tree_el == NULL)
		return (toplace);
	if (index < tree_el->branch)
		tree_el->left = place_in_tree(tree_el->left, toplace, index);
	else if (index > tree_el->branch)
		tree_el->right = place_in_tree(tree_el->right, toplace, index);
	return (tree_el);
}

t_btree	*btree_new(t_token	*tkn_toconvert)
{
	t_btree		*tree_el;

	tkn_toconvert->used_flag = 1;
	tree_el = calloc_gc(1, sizeof(t_btree), B_TREE);
	if (!tree_el)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	tree_el->branch = tkn_toconvert->index;
	tree_el->type = tkn_toconvert->type;
	if (is_cmd_tkn(tkn_toconvert->type))
	{
		tree_el->type = WORD;
		tree_el->cmds = extract_cmd_argv(tkn_toconvert);
	}
	else
		tree_el->cmds = strdup_in_newchar2(tkn_toconvert->content);
	return (tree_el);
}

t_btree	*create_bin_tree(t_tknlist *tknlst)
{
	t_token	*prio_tkn;
	t_btree	*btree_root;

	if (g_status != 0)
		return (NULL);
	btree_root = NULL;
	while (tknlst->head)
	{
		if (tknlst->head->used_flag == 1 || tknlst->head->content[0] == 0)
		{
			pop_token_in_place(tknlst, tknlst->head);
			continue ;
		}
		prio_tkn = find_prior_token(tknlst->head);
		if (!btree_root)
			btree_root = btree_new(prio_tkn);
		else
			place_in_tree(btree_root, btree_new(prio_tkn), prio_tkn->index);
		prio_tkn->used_flag = 1;
	}
	clear_garbage(TKN_LIST, free);
	return (btree_root);
}
