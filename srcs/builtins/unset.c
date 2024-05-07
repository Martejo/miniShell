/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 13:32:40 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 10:33:12 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	env_size(char *env)
{
	size_t		i;

	if (!env)
		return (0);
	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (i);
}

int	search_and_unset(char **value, t_env **env, int index)
{
	t_env	*tmp;
	t_env	*curr;
	t_env	*node;

	curr = *env;
	while (curr && curr->next)
	{
		if (ft_strncmp(value[index], curr->next->value,
				env_size(curr->next->value)) == 0)
		{
			tmp = curr->next->next;
			node = curr->next;
			del_one_garbage(node->value, ENV);
			del_one_garbage(node, ENV);
			curr->next = tmp;
			return (0);
		}
		curr = curr->next;
	}
	return (0);
}

int	unset(char **value, t_env **env)
{
	t_env	*tmp;
	size_t	i;

	if (!(value[1]) || !(*env))
		return (0);
	i = 0;
	while (value[++i])
	{
		if (value[i][0] == 0)
			return (print_without_exit \
				("Minishell: unset: `': not a valid identifier\n", RED, 1), 1);
		if (ft_strncmp(value[i], (*env)->value
				, (int)env_size((*env)->value)) == 0)
		{
			tmp = *env;
			if ((*env)->next)
				*env = (*env)->next;
			else
				*env = NULL;
			del_one_garbage(tmp, ENV);
		}
		else
			search_and_unset(value, env, i);
	}
	return (0);
}
