/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reducer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:01:37 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:04:39 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	type_need_reducing(t_tkntype type)
{
	if (type == IN || type == OUT || type == APPEND \
		|| type == HEREDOC || type == TWO_QUOTE \
		|| type == ONE_QUOTE || type == PARENTHESIS)
		return (1);
	return (0);
}

static char	*reduce_content(t_token *tkn)
{
	char	*new_content;

	if (tkn->type == IN || tkn->type == OUT)
	{
		new_content = remove_substr(tkn->content, 0, 1 \
			+ ft_strlen_until_not(&tkn->content[1], ft_isspace));
		if (new_content[0] == '"')
			new_content = ft_strcut(new_content, 1, 1);
	}
	if (tkn->type == HEREDOC || tkn->type == APPEND)
		new_content = remove_substr(tkn->content, 0, 2 \
			+ ft_strlen_until_not(&tkn->content[2], ft_isspace));
	else if (tkn->type == TWO_QUOTE \
		|| tkn->type == ONE_QUOTE || tkn->type == PARENTHESIS)
		new_content = ft_strcut(tkn->content, 1, 1);
	return (new_content);
}

void	reducer(t_token	*tkn)
{
	char	*new_content;

	if (g_status != 0)
		return ;
	while (tkn)
	{
		if (type_need_reducing(tkn->type))
		{
			new_content = reduce_content(tkn);
			if (!new_content)
				print_and_exit(MALLOC_ERR_MSG, RED, 1);
			add_to_garbage(new_content, TKN_LIST);
			del_one_garbage(tkn->content, TKN_LIST);
			tkn->content = new_content;
		}
		tkn = tkn->next;
	}
}
