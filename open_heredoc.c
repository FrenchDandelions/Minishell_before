/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 17:07:17 by thole             #+#    #+#             */
/*   Updated: 2024/02/27 17:07:18 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// extern int	g_sig;

int	end_heredoc(t_last_list **list, char *buf, char *str)
{
	free(str);
	free((*list)->next->str);
	if (buf)
	{
		(*list)->next->str = ft_strdup(buf);
		if (!(*list)->next->str)
			return (free(buf), ERR_MALLOC);
		free(buf);
	}
	else
	{
		(*list)->next->str = ft_strdup("");
		if (!(*list)->next->str)
			return (ERR_MALLOC);
	}
	return (SUCCESS);
}

int	add_to_buffer(char **buf, char *str)
{
	if ((*buf))
	{
		(*buf) = ft_gnl_strjoin((*buf), str, (int)ft_strlen(str));
		if (!(*buf))
			return (ERR_MALLOC);
	}
	else
	{
		(*buf) = ft_strdup(str);
		if (!(*buf))
			return (free(str), ERR_MALLOC);
	}
	return (SUCCESS);
}

void	heredoc_sig(int code)
{
	(void)code;
	g_sig = 130;
	// ft_dprintf(STDIN_FILENO, "\n");
	printf("\n");
	close(STDIN_FILENO);
	// rl_on_new_line();
	// rl_replace_line("", 0);
	// rl_redisplay();
}

int	open_heredoc(t_last_list **list, char *lim)
{
	char	*str;
	char	*buf;
	int		fd;

	fd = dup(STDIN_FILENO);
	buf = NULL;
	signal(SIGINT, &heredoc_sig);
	while (1)
	{
		str = readline(PROMPT_HD);
		if (!str || g_sig == 130)
		{
			if (g_sig == 130)
			{
				if (str)
					free(str);
				if (buf)
					ft_memdel(buf);
				g_sig = 0;
				dup2(fd, STDIN_FILENO);
				close(fd);
				return (QUIT);
			}
			return (ft_dprintf(STDERR_FILENO, "%s%s')\n\033[0m", ERR_MINI_DOC,
					lim), end_heredoc(&(*list), buf, str));
		}
		if (!str[0])
		{
			free(str);
			str = ft_strdup("\n");
			if (!str)
				return (ERR_MALLOC);
		}
		if (ft_strncmp(str, lim, (int)ft_strlen(str)) == 0)
			return (end_heredoc(&(*list), buf, str));
		str = ft_gnl_strjoin(str, "\n", 1);
		if (add_to_buffer(&buf, str) == ERR_MALLOC)
			return (ERR_MALLOC);
		free(str);
	}
	return (0);
}
