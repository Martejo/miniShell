/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:18 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:20 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	sort_tknlst_like_wildcard(t_tknlist *list_expnd)
{
	t_token		*curr;
	t_token		*next;
	size_t		i;
	size_t		lst_size;

	i = 0;
	lst_size = tknlst_size(list_expnd);
	while (i < lst_size)
	{
		curr = list_expnd->head;
		while (curr)
		{
			next = curr->next;
			if (next \
				&& ft_strcmp_case_insensitive(curr->content, next->content) > 0)
			{
				swap_tokens(list_expnd, curr, next);
				continue ;
			}
			curr = curr->next;
		}
		i++;
	}
}

static int	is_compatible_file_wildcard(char *file, \
	char **subs_needed, char *to_expand)
{
	if (to_expand[0] == '*' && file[0] == '.')
		return (0);
	if (to_expand[0] != '*' && !s1_is_s2_prefix(subs_needed[0], file))
		return (0);
	if (!str_contains_all_subs_ordered(file, subs_needed))
		return (0);
	if (to_expand[ft_strlen(to_expand) - 1] != '*' \
		&& !s1_is_s2_suffix(subs_needed[char2len(subs_needed) - 1], file))
		return (0);
	return (1);
}

static void	lstadd_wildcard_expansions(t_tknlist *wildcard_lst, \
	char **subs_needed, char *to_expand)
{
	DIR				*dir;
	struct dirent	*entry;
	t_token			*new_tkn;

	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (is_compatible_file_wildcard(entry->d_name, subs_needed, to_expand))
		{
			new_tkn = create_node(WORD, strdup_gc(entry->d_name, TKN_LIST), 0);
			if (!new_tkn)
			{
				closedir(dir);
				print_and_exit(MALLOC_ERR_MSG, RED, 1);
			}
			add_node(wildcard_lst, new_tkn);
		}
		entry = readdir(dir);
	}
	if (dir)
		closedir(dir);
}

t_token	*expand_wildcard(t_token *tkn_toexpand, t_tknlist *tkn_lst)
{
	t_tknlist	*wildcard_lst;
	char		**splitted;

	splitted = split_gc(tkn_toexpand->content, '*', TMP);
	init_list(&wildcard_lst);
	lstadd_wildcard_expansions(wildcard_lst, splitted, tkn_toexpand->content);
	if (!wildcard_lst->head)
		return (tkn_toexpand);
	sort_tknlst_like_wildcard(wildcard_lst);
	add_tknlst_in_tknlst_after_target(tkn_lst, tkn_toexpand, wildcard_lst);
	pop_token_in_place(tkn_lst, tkn_toexpand);
	tkn_toexpand = wildcard_lst->head;
	clear_garbage(TMP, free);
	return (wildcard_lst->tail);
}
