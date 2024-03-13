/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_test.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 15:51:18 by thole             #+#    #+#             */
/*   Updated: 2024/03/13 15:51:28 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char	*ft_strnjoin(char *s1, char *s2, size_t size_s1, int start);

int	is_alnum_undescore(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*find_in_path(char *needle, char **env)
{
	int		size;
	int		i;
	char	*str;

	size = (int)ft_strlen(needle);
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], needle, size))
		{
			str = ft_strdup(env[i] + 1 + size);
			return (str);
		}
		i++;
	}
	if (size == 1)
		return (ft_strdup("$"));
	return (ft_strdup(""));
}

char	*expand(char *str, char **env)
{
	char	*string;
	char	*dup;
	char	*dup2;
	int		i;
	int		len;
	int		oldlen;
	int		j;

	dup = NULL;
	j = 0;
	i = 0;
	oldlen = ft_strlen(str);
	string = calloc(sizeof(char), (oldlen + 1));
	while (str[i])
	{
		if (str[i] == '\'')
		{
			ft_sprintf(string + j, "%s", &str[i]);
			i++;
			j++;
			while (str[i] != '\'')
			{
				ft_sprintf(string + j, "%s", &str[i]);
				i++;
				j++;
			}
			ft_sprintf(string + j, "%s", &str[i]);
			i++;
			j++;
		}
		if (str[i] == '\"')
		{
			ft_sprintf(string + j, "%s", &str[i]);
			i++;
			j++;
			while (str[i] != '\"')
			{
				if (str[i] == '$')
				{
					len = 0;
					i++;
					while (is_alnum_undescore(str[i + len]))
						len++;
					dup = ft_substr(str, i, len);
					if (!dup)
						return (NULL);
					dup2 = find_in_path(dup, env);
					string = ft_realloc(string, sizeof(char) * (ft_strlen(dup2)
								+ ft_strlen(string) + ft_strlen(str)),
							ft_strlen(string));
					if (!string)
						return (NULL);
					ft_sprintf(string + j, "%s", dup2);
					j += (int)ft_strlen(dup2);
					i += len;
					free(dup);
				}
				if (str[i] && str[i] != '$' && str[i] != '\"')
				{
					ft_sprintf(string + j, "%s", &str[i]);
					i++;
					j++;
				}
				printf("Here : %c\n", str[i]);
			}
			ft_sprintf(string + j, "%s", &str[i]);
			i++;
			j++;
		}
		if (str[i] == '$')
		{
			len = 0;
			i++;
			while (is_alnum_undescore(str[i + len]))
				len++;
			dup = ft_substr(str, i, len);
			if (!dup)
				return (NULL);
			dup2 = find_in_path(dup, env);
			string = ft_realloc(string, sizeof(char) * (ft_strlen(dup2)
						+ ft_strlen(string) + ft_strlen(str)),
					ft_strlen(string));
			if (!string)
				return (NULL);
			ft_sprintf(string + j, "%s", dup2);
			j += (int)ft_strlen(dup2);
			i += len;
			free(dup);
		}
		if (str[i] && str[i] != '$')
		{
			ft_sprintf(string + j, "%s", &str[i]);
			i++;
			j++;
		}
	}
	return (string);
}

int	main(int argc, char **argv, char **env)
{
	char	*str;
	char	*str2;

	(void)argc;
	(void)argv;
	while (1)
	{
		str2 = readline("Test>>> ");
		if (!str2)
			break ;
		add_history(str2);
		printf("Coucou %s\n", str2);
		str = expand(str2, env);
		printf("Here : %s\n", str);
		free(str2);
		free(str);
	}
}
