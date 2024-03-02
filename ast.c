/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 16:51:43 by thole             #+#    #+#             */
/*   Updated: 2024/02/29 16:51:44 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_tree	*build_root(void)
{
	t_tree	*tree;

	tree = (t_tree *)malloc(sizeof(t_tree));
	if (!tree)
		return (NULL);
	tree->left = NULL;
	tree->right = NULL;
	tree->cmd_tab = NULL;
	tree->infile = NULL;
	tree->outfile = NULL;
	tree->file = (t_file *)malloc(sizeof(t_file));
	if (!tree->file)
		return (free(tree), NULL);
	tree->file->next = NULL;
	tree->file->prev = NULL;
	tree->err_code = 0;
	return (tree);
}

int	build_ast(t_struct *s)
{
	t_tree	*root;

	root = build_root();
	if (!root)
		return (ERR_MALLOC);
}
