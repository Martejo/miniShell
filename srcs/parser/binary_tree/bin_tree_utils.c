/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_tree_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:00:56 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:00:58 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

size_t	count_argc_cmd(t_token *curr)
{
	size_t	argc;

	argc = 0;
	while (curr && is_cmd_tkn(curr->type))
	{
		argc++;
		curr = curr->next;
	}
	return (argc);
}

char	**get_argv_cmd(t_token *curr, size_t argc)
{
	char	**argv;
	size_t	i;

	argv = (char **) calloc_gc(argc + 1, sizeof(char *), B_TREE);
	if (!argv)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	i = 0;
	while (i < argc)
	{
		curr->used_flag = 1;
		argv[i] = strdup_gc(curr->content, B_TREE);
		if (!argv[i])
			print_and_exit(MALLOC_ERR_MSG, RED, 1);
		curr = curr->next;
		i++;
	}
	return (argv);
}

char	**strdup_in_newchar2(char *delimiter)
{
	char	**cpy;

	cpy = (char **)calloc_gc(2, sizeof(char *), B_TREE);
	if (!cpy)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	cpy[0] = strdup_gc(delimiter, B_TREE);
	if (!cpy[0])
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	return (cpy);
}

char	**extract_cmd_argv(t_token *curr)
{
	size_t	argc;
	char	**argv;

	argc = count_argc_cmd(curr);
	argv = get_argv_cmd(curr, argc);
	if (!argv)
		print_and_exit(MALLOC_ERR_MSG, RED, 1);
	return (argv);
}

int	cmd_is_inside_pipe(t_btree *root, int branch_id)
{
	t_btree	*curr;

	curr = root;
	while (curr && curr->branch != branch_id)
	{
		if (curr->type == PIPE)
			return (1);
		if (branch_id < curr->branch)
			curr = curr->left;
		else
			curr = curr->right;
	}
	return (0);
}
