/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 16:07:05 by thole             #+#    #+#             */
/*   Updated: 2024/03/14 16:07:06 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	free_stack_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i++]);
}

void	free_all(t_struct *s, int ex)
{
	if (s->tab)
		free_tab(s->tab);
	free_env(s->env);
	free_env(s->dup_env);
	if (s->infile)
		free(s->infile);
	if (s->outfile)
		free(s->outfile);
	free(s->str);
	if (s->file->files)
		flush_files(s->file, s);
	ft_free_changed_list(s->head_ll);
	if (s->count_pipes)
	{
		close(s->pipe[0]);
		close(s->pipe[1]);
	}
	exit(ex);
}
