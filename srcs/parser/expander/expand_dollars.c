/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:11 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:01:13 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	is_charset_env(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

static	t_token	*dollar_detected(t_mini *mini, char *to_expand, \
	size_t *var_len, t_tkntype type)
{
	if (to_expand[1] == '?')
	{
		*var_len = 1;
		return (create_node(type, \
				itoa_gc(mini->last_gstatus, TKN_LIST), 1));
	}
	else if (ft_isdigit(to_expand[1]))
	{
		*var_len = 1;
		return (create_node(type, "", 1));
	}
	else if (!is_charset_env(to_expand[1]))
	{
		*var_len = 1;
		return (create_node(type, \
				strndup_gc(to_expand, *var_len + 1, TKN_LIST), 1));
	}
	*var_len = ft_strlen_until_not(&to_expand[1], is_charset_env);
	return (create_node(type, \
			get_env_value(mini->env, &to_expand[1], *var_len), 1));
}

static	t_token	*dollar_undetected(char *to_expand, \
	size_t *var_len, t_tkntype type)
{
	*var_len = ft_strlen_until_char(&to_expand[0], '$') - 1;
	return (create_node(type,
			strndup_gc(to_expand, *var_len + 1, TKN_LIST), 1));
}

static void	lstadd_dollar_expansions(t_mini *mini, \
	t_tkntype tkntype, char *to_expand, t_tknlist *dollar_lst)
{
	size_t	i;
	t_token	*new_tkn;
	size_t	len_sub;

	i = 0;
	len_sub = 0;
	while (to_expand[i])
	{
		new_tkn = NULL;
		if (to_expand[i] == '$')
			new_tkn = dollar_detected(mini, &to_expand[i], &len_sub, tkntype);
		else
			new_tkn = dollar_undetected(&to_expand[i], &len_sub, tkntype);
		if (!new_tkn)
			print_and_exit(MALLOC_ERR_MSG, RED, MALLOC_E);
		add_node(dollar_lst, new_tkn);
		i += len_sub + 1;
	}
}

t_token	*expand_dollar(t_mini *mini, t_token *tkn_toexpand, t_tknlist *tkn_lst)
{
	t_tknlist	*dollar_lst;

	init_list(&dollar_lst);
	lstadd_dollar_expansions(mini, tkn_toexpand->type, \
		tkn_toexpand->content, dollar_lst);
	dollar_lst->tail->link = tkn_toexpand->link;
	add_tknlst_in_tknlst_after_target(tkn_lst, tkn_toexpand, dollar_lst);
	pop_token_in_place(tkn_lst, tkn_toexpand);
	return (dollar_lst->tail);
}
