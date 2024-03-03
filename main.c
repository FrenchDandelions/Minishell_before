/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 12:54:07 by thole             #+#    #+#             */
/*   Updated: 2024/02/04 12:54:09 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	ft_free_parse_list(t_parse_list *list)
{
	t_parse_list	*current;
	t_parse_list	*temp;

	current = list;
	while (current)
	{
		temp = current->next;
		free(current);
		current = temp;
	}
}

void	ft_free_changed_list(t_last_list *list)
{
	t_last_list	*current;
	t_last_list	*temp;

	current = list;
	while (current)
	{
		temp = current->next;
		if (current->str)
			free(current->str);
		free(current);
		current = temp;
	}
}

void	ft_print_list2(t_last_list *lst)
{
	t_last_list	*current;

	current = lst;
	while (current)
	{
		if (!current->prev)
			printf("Bam!\n");
		printf("String : %s, token : %d\n", current->str, current->token);
		current = current->next;
	}
}
void	ft_print_list(t_parse_list *lst)
{
	t_parse_list	*current;

	current = lst;
	while (current->next)
	{
		if (!current->prev)
			printf("Bam!\n");
		printf("Char : %c, Token : %d\n", current->c, current->token);
		current = current->next;
	}
	printf("Char : %c, Token : %d\n", current->c, current->token);
	printf("\nAnd now the other way!\n\n");
	while (current->prev)
	{
		if (!current->next)
			printf("Bim!\n");
		current = current->prev;
		printf("Char : %c, Token : %d\n", current->c, current->token);
	}
}

int	quote_checker(char *str)
{
	int		i;
	char	type;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			type = str[i];
			i++;
			while (str[i] && str[i] != type)
				i++;
		}
		if (!str[i] && type == '\'')
			return (ERR_SINGLE);
		else if (!str[i] && type == '\"')
			return (ERR_DOUBLE);
		i++;
	}
	return (SUCCESS);
}

int	check_parenthesis(char *str)
{
	int		i;
	int		nb;
	char	type;

	i = 0;
	nb = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			type = str[i];
			i++;
			while (str[i] && str[i] != type)
				i++;
		}
		if (str[i] == '(')
			nb++;
		else if (str[i] == ')')
			nb--;
		i++;
	}
	if (nb != 0)
		return (ERR_PARS);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_struct	s;
	char		*str;
	int			err;
	int			err2;

	err = 0;
	str = NULL;
	(void)argv;
	(void)argc;
	while (1)
	{
		str = readline("\033[1;34mMinishell$\033[0m ");
		if (!str)
		{
			rl_clear_history();
			clear_history();
			return (printf("Bye ;)\n"), exit(0), SUCCESS);
		}
		add_history((const char *)str);
		if (quote_checker(str) < SUCCESS)
		{
			printf("Error\n");
			free(str);
			continue ;
		}
		if (check_parenthesis(str) == ERR_PARS)
		{
			free(str);
			printf("Error2\n");
			continue ;
		}
		s.str = strdup(str);
		s.err = 0;
		ft_prototype_list(&s);
		s.head_parse = s.p_lst;
		// ft_print_list(s.head_parse);
		ft_change_list(&s);
		s.head_ll = s.l_lst;
		s.p_lst = s.head_parse;
		s.temp = s.head_ll;
		err = parser(&s);
		err2 = parse_heredoc(&s);
		if (err2 == ERR_MALLOC || err == ERR_MALLOC)
		{
			dprintf(STDERR_FILENO, "ERR_MALLOC");
			ft_free_parse_list(s.p_lst);
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			continue ;
		}
		if (err == ERR_PARS || err == ERR_MALLOC)
		{
			ft_free_parse_list(s.p_lst);
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			continue ;
		}
		// ft_print_list2(s.l_lst);
		execute(&s, s.head_ll);
		ft_free_parse_list(s.p_lst);
		ft_free_changed_list(s.l_lst);
		free(s.str);
		free(str);
	}
	return (0);
}
