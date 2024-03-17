#include "test.h"

int	is_quotes(int c)
{
	if (c == '\'' || c == '\"')
		return (1);
	return (0);
}

char	*takeoff_quotes(char *str)
{
	char	*dup;
	char	type;
	int		i;
	int		j;

	dup = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!dup)
		return (free(str), NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (is_quotes(str[i]))
		{
			type = str[i++];
			while (str[i] && str[i] != type)
				dup[j++] = str[i++];
			if (str[i] == type)
				i++;
		}
		if (str[i] && !is_quotes(str[i]))
			dup[j++] = str[i++];
	}
	return (free(str), dup);
}

int	is_alnum_undescore(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*find_in_path(char *needle, char **env, int len, t_struct *s)
{
	int		size;
	int		i;
	char	*str;

	size = (int)ft_strlen(needle);
	i = 0;
	if (len == 1 && size == 0)
		return (ft_strdup(""));
	if (size == 1 && needle[0] == '?')
		return (ft_itoa(s->exit_val));
	while (env[i] && needle[0])
	{
		if (ft_strnstr(env[i], needle, size))
		{
			str = ft_strdup(env[i] + 1 + size);
			return (str);
		}
		i++;
	}
	if (len == 0 && size == 0)
		return (ft_strdup("$"));
	return (ft_strdup(""));
}

char	*expand(char *str, char **env, t_struct *s)
{
	char	*string;
	char	*dup;
	char	*dup2;
	int		i;
	int		len;
	int		j;

	dup = NULL;
	dup2 = NULL;
	j = 0;
	i = 0;
	string = ft_calloc(sizeof(char), (ft_strlen(str) + 1));
	if (!string)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\'')
		{
			ft_sprintf(string + j, "%c", str[i]);
			i++;
			j++;
			while (str[i] && str[i] != '\'')
			{
				ft_sprintf(string + j, "%c", str[i]);
				i++;
				j++;
			}
			if (str[i])
			{
				ft_sprintf(string + j, "%c", str[i]);
				i++;
				j++;
			}
		}
		if (str[i] == '\"')
		{
			ft_sprintf(string + j, "%c", str[i]);
			i++;
			j++;
			while (str[i] && str[i] != '\"')
			{
				if (str[i] == '$')
				{
					len = 0;
					i++;
					while (str[i + len] && (is_alnum_undescore(str[i + len])
							|| (len == 0 && str[i + len] == '?')))
					{
						if (len == 1 && str[i + len - 1] == '?')
							break ;
						len++;
					}
					if (len == 0)
					{
						if (!str[i])
						{
							dup = ft_strdup("");
							if (!dup)
								return (free(string), NULL);
							dup2 = find_in_path(dup, env, 1, s);
							if (!dup2)
								return (free(dup), free(string), NULL);
						}
						if (str[i])
						{
							dup = ft_strdup("");
							if (!dup)
								return (free(string), NULL);
							dup2 = find_in_path(dup, env, 0, s);
							if (!dup2)
								return (free(dup), free(string), NULL);
						}
					}
					else
					{
						dup = ft_substr(str, i, len);
						if (!dup)
							return (free(string), NULL);
						dup2 = find_in_path(dup, env, 1, s);
						if (!dup2)
							return (free(dup), free(string), NULL);
					}
					string = ft_realloc(string, sizeof(char) * (ft_strlen(dup2)
								+ ft_strlen(string) + ft_strlen(str)),
							ft_strlen(string));
					if (!string)
						return (NULL);
					ft_sprintf(string + j, "%s", dup2);
					j += (int)ft_strlen(dup2);
					i += len;
					free(dup);
					dup = NULL;
					free(dup2);
					dup2 = NULL;
				}
				if (str[i] && str[i] != '$' && str[i] != '\"')
				{
					ft_sprintf(string + j, "%c", str[i]);
					i++;
					j++;
				}
			}
			if (str[i])
			{
				ft_sprintf(string + j, "%c", str[i]);
				i++;
				j++;
			}
		}
		if (str[i] == '$')
		{
			len = 0;
			i++;
			while (str[i + len] && (is_alnum_undescore(str[i + len])
					|| (len == 0 && str[i + len] == '?')))
			{
				if (len == 1 && str[i + len - 1] == '?')
					break ;
				len++;
			}
			if (len == 0)
			{
				if (!str[i])
				{
					dup = ft_strdup("");
					if (!dup)
						return (free(string), NULL);
					dup2 = find_in_path(dup, env, 0, s);
					if (!dup2)
						return (free(dup), free(string), NULL);
				}
				if (str[i] && is_quotes(str[i]))
				{
					dup = ft_strdup("");
					if (!dup)
						return (free(string), NULL);
					dup2 = find_in_path(dup, env, 1, s);
					if (!dup2)
						return (free(dup), free(string), NULL);
				}
			}
			else
			{
				dup = ft_substr(str, i, len);
				if (!dup)
					return (free(string), NULL);
				dup2 = find_in_path(dup, env, 1, s);
				if (!dup2)
					return (free(dup), free(string), NULL);
			}
			string = ft_realloc(string, sizeof(char) * (ft_strlen(dup2)
						+ ft_strlen(string) + ft_strlen(str)),
					ft_strlen(string));
			if (!string)
				return (NULL);
			ft_sprintf(string + j, "%s", dup2);
			j += (int)ft_strlen(dup2);
			i += len;
			free(dup);
			dup = NULL;
			free(dup2);
			dup2 = NULL;
		}
		if (str[i] && str[i] != '$')
		{
			ft_sprintf(string + j, "%c", str[i]);
			i++;
			j++;
		}
	}
	return (string);
}
