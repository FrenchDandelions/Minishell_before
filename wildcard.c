/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 14:01:38 by thole             #+#    #+#             */
/*   Updated: 2024/03/08 14:01:39 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*do_expand(char *file, int *j)
{
	int		i;
	char	type;
	int		len;
	char	*s;

	len = (int)ft_strlen(file);
	i = 0;
	type = 'c';
	s = calloc(sizeof(char), len + 1);
	if (!s)
		return (NULL);
	if (*j > len)
		return (s);
    while((j + i) <= len)
    {
        if(file[(*j) + i] == '\'')
            
    }
	return (s);
}

// char	*do_expand(char *file, int *j)
// {
// 	int		i;
// 	char	type;
// 	char	*s;

// 	i = 0;
// 	type = 'c';
// 	s = calloc(sizeof(char), ((int)strlen(file) + 1));
// 	if (!s)
// 		return (NULL);
// 	if (file[(*j)] && file[(*j)] == '*')
// 	{
// 		s[0] = file[(*j)];
// 		while (file[(*j)] && file[(*j)] == '*')
// 			(*j)++;
// 		return (s);
// 	}
// 	while ((*j) < (int)ft_strlen(file) && file[(*j)] && file[(*j) + i])
// 	{
// 		if ((*j) < (int)ft_strlen(file) && file[(*j) + i] && file[(*j)
// 			+ i] == '\'')
// 		{
// 			(*j)++;
// 			type = '\'';
// 		}
// 		else if ((*j) < (int)ft_strlen(file) && file[(*j) + i] && file[(*j)
// 			+ i] == '\"')
// 		{
// 			(*j)++;
// 			type = '\"';
// 		}
// 		else if ((*j) < (int)ft_strlen(file) && file[(*j) + i] && file[(*j)
// 			+ i] == '*')
// 			break ;
// 		if (type == '\'' || type == '\"')
// 		{
// 			while ((*j) < (int)ft_strlen(file) && file[(*j) + i] && file[(*j)
// 				+ i] != type)
// 			{
// 				s[i] = file[i + (*j)];
// 				i++;
// 			}
// 			if ((*j) < (int)ft_strlen(file) && file[(*j) + i])
// 				i++;
// 			type = 'c';
// 		}
// 		printf("Here bro? %d, %c\n", *j + i, file[(*j) + i]);
// 		s[i] = file[(*j) + i];
// 		i++;
// 	}
// 	(*j) = (*j) + i;
// 	if ((*j) < (int)ft_strlen(file))
// 		printf("Here :%c\n", file[(*j) + i]);
// 	return (s);
// }

int	check_expand(char *file, int *size)
{
	int		i;
	char	type;

	i = 0;
	type = 'c';
	while (file[i])
	{
		if (file[i] && file[i] == '\'')
			type = '\'';
		else if (file[i] && file[i] == '\"')
			type = '\"';
		else if (file[i] && file[i] == '*')
			(*size)++;
		if (file[i] && file[i] == '*')
			while (file[i] && file[i] == '*')
				i++;
		else
			i++;
		if (type == '\'' || type == '\"')
		{
			while (file[i] && file[i] != type)
				i++;
			i++;
			type = 'c';
		}
	}
	return (SUCCESS);
}

char	**split_wildcards(char *s)
{
	(void)s;
	return (NULL);
}

char	**wildcards_expands(char **tab)
{
	int				size_tab;
	int				i;
	struct dirent	*init_files;
	DIR				*dir;

	i = 0;
	size_tab = 0;
	while (tab[size_tab])
		size_tab++;
	dir = opendir(".");
	if (!dir)
		return (tab);
	while ((init_files = readdir(dir)) != NULL)
	{
		;
	}
	closedir(dir);
	return (tab);
}

int	main(void)
{
	char	*str;
	int		stat;
	int		size;
	char	*new_str;
	int		index;
	char	**expand_table;
	int		i;

	while (1)
	{
		index = 0;
		size = 0;
		i = 0;
		str = readline("Test bro > ");
		if (!str)
			return (printf("Bye ;)\n"), 1);
		add_history(str);
		stat = check_expand(str, &size);
		if (size)
			printf("Wildcard found! Go expand :D! %d\n", size);
		else
		{
			printf("No wildcard found :( %d\n", size);
		}
		printf("Here bro?\n");
		expand_table = malloc(sizeof(char *) * ((size + 1) * 3));
		printf("Here bro?\n");
		while (str[index])
		{
			printf("Here bro? %d\n", index);
			new_str = do_expand(str, &index);
			if (!new_str)
				printf("ERR_MALLOC\n");
			expand_table[i] = ft_strdup(new_str);
			expand_table[i + 1] = NULL;
			free(new_str);
			i++;
			if (index > (int)ft_strlen(str))
				break ;
		}
		// expand_table[i] = NULL;
		i = 0;
		while (expand_table[i])
		{
			printf("tab: %s\n", expand_table[i]);
			free(expand_table[i]);
			i++;
		}
		free(expand_table);
	}
}
