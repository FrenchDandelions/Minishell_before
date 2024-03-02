/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 16:48:21 by thole             #+#    #+#             */
/*   Updated: 2024/02/27 16:48:22 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

char	*ft_gnl_strchr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

size_t	ft_gnl_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

size_t	ft_gnl_strlcpy(char *dest, char *src, size_t destsize)
{
	size_t	srcsize;
	size_t	i;

	i = 0;
	srcsize = ft_gnl_strlen(src);
	if (destsize > 0)
	{
		while (i < srcsize && i < destsize - 1)
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (srcsize);
}

char	*ft_gnl_strdup(char *s)
{
	char	*copy;
	size_t	len;

	len = ft_gnl_strlen(s) + 1;
	copy = malloc(sizeof(char) * (len));
	if (!copy)
		return (NULL);
	ft_gnl_strlcpy(copy, s, len);
	return (copy);
}

char	*ft_gnl_strjoin(char *s1, char *s2, size_t len)
{
	size_t	len1;
	size_t	len2;
	char	*s3;

	len1 = ft_gnl_strlen(s1);
	len2 = len;
	s3 = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!s3)
		return (NULL);
	ft_gnl_strlcpy(s3, s1, len1 + 1);
	ft_gnl_strlcpy((s3 + len1), s2, len2 + 1);
	free(s1);
	return (s3);
}

void	do_new_line(char **line, char *stock)
{
	int		to_copy;
	char	*new_line;

	new_line = ft_gnl_strchr((*line), '\n');
	if (new_line != NULL)
	{
		to_copy = new_line - (*line) + 1;
		ft_gnl_strlcpy(stock, new_line + 1, BUFFER_SIZE + 1);
	}
	else
	{
		to_copy = ft_gnl_strlen((*line));
		ft_gnl_strlcpy(stock, "", BUFFER_SIZE + 1);
	}
	(*line)[to_copy] = '\0';
}

char	*get_next_line(int fd)
{
	static char	stock[BUFFER_SIZE + 1];
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = ft_gnl_strdup(stock);
	bytes_read = 1;
	while (!(ft_gnl_strchr(line, '\n')) && bytes_read > 0)
	{
		bytes_read = read(fd, stock, BUFFER_SIZE);
		stock[bytes_read] = '\0';
		line = ft_gnl_strjoin(line, stock, BUFFER_SIZE);
	}
	if (ft_gnl_strlen(line) == 0)
		return (free(line), NULL);
	do_new_line(&line, stock);
	return (line);
}
