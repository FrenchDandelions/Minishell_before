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

int		g_sig;

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

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i] = NULL;
		i++;
	}
	free(env);
	env = NULL;
}
int	get_env(char **env, t_struct *s)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	s->env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!s->env)
		return (ERR_MALLOC);
	i = 0;
	while (env[i])
	{
		s->env[i] = ft_strdup(env[i]);
		if (!s->env[i])
			return (free_env(s->env), ERR_MALLOC);
		i++;
	}
	s->env[i] = NULL;
	s->size_env = i;
	return (SUCCESS);
}

int	wait_pid(t_struct *s)
{
	int	status;
	int	ret;

	while (errno != ECHILD)
	{
		if (wait(&status) == s->pid)
		{
			if (WIFEXITED(status))
				ret = WEXITSTATUS(status);
			else
				ret = 128 + WTERMSIG(status);
			if (ret == 131)
				ft_dprintf(2, "Quit (core dumped)\n");
			else if (ret == 130)
				ft_dprintf(2, "\n");
		}
	}
	g_sig = 0;
	return (ret);
}

int	is_token(int token)
{
	if (token == TK_AND || token == TK_OR || token == TK_DLMTR
		|| token == TK_SINGLE || token == TK_PIPES || token == TK_PRIO
		|| token == TK_END_PRIO || token == TK_APPEND || token == TK_ARG
		|| token == TK_AND || token == TK_AND || token == TK_WILD
		|| token == TK_DOUBLE || token == TK_OUTPUT || token == TK_INPUT
		|| token == TK_EXPAND || token == TK_ENV_VAR)
		return (1);
	return (0);
}

int	check_list(t_last_list *list)
{
	t_last_list	*temp;
	int			i;

	temp = list;
	i = 0;
	while (temp->next)
	{
		if (is_token(temp->token))
			i++;
		temp = temp->next;
	}
	return (i);
}

int	main(int argc, char **argv, char **env)
{
	t_struct	s;
	char		*str;
	int			err;
	int			err2;
	int			stat;

	stat = 0;
	g_sig = 0;
	str = NULL;
	if (isatty(STDIN_FILENO) == 0)
		return (ft_dprintf(STDERR_FILENO, "Wrong STDIN\n"), EXIT_FAILURE);
	(void)argv;
	(void)argc;
	if (!env[0])
		return (ft_dprintf(STDERR_FILENO, "No env detected\n"), 1);
	s.exit_arg = 0;
	if (get_env(env, &s) == ERR_MALLOC)
		return (ft_dprintf(2, "Malloc\n"), -2);
	s.dup_env = dup_array(s.env);
	if (!s.dup_env)
		return (ft_dprintf(2, "Malloc\n"), -2);
	s.err = NOTHING;
	s.exit_val = 0;
	s.bad_exit = 0;
	while (1 && stat != EXIT)
	{
		s.exit = 0;
		s.exported = 0;
		s.unsetted = 0;
		s.bad_exit = 0;
		s.is_first = 1;
		s.is_last = 0;
		s.num_err_exit = 0;
		sig_init(&s);
		str = readline(PROMPT);
		if (!str)
		{
			rl_clear_history();
			free_env(s.env);
			free_env(s.dup_env);
			ft_dprintf(2, "\033[1;95mexit\n\033[0m");
			if (g_sig == 130)
				s.exit_val = 130;
			else if (g_sig == 131)
				s.exit_val = 131;
			break ;
		}
		if (g_sig == 130)
		{
			s.exit_val = 130;
			g_sig = 0;
		}
		add_history((const char *)str);
		if (quote_checker(str) < SUCCESS)
		{
			printf("%s", ERR_QUOTES);
			free(str);
			continue ;
		}
		if (check_parenthesis(str) == ERR_PARS)
		{
			free(str);
			printf("%s", ERR_PARENTHESIS);
			continue ;
		}
		s.str = ft_strdup(str);
		if (!s.str)
		{
			ft_dprintf(2, "Malloc\n");
			free(str);
			s.exit_val = -2;
			continue ;
		}
		s.err = 0;
		if (ft_prototype_list(&s) == ERR_MALLOC)
		{
			ft_dprintf(2, "Malloc\n");
			free(s.str);
			free(str);
			s.exit_val = -2;
			continue ;
		}
		s.head_parse = s.p_lst;
		// ft_print_list(s.head_parse);
		if (ft_change_list(&s) == ERR_MALLOC)
		{
			ft_dprintf(2, "Malloc\n");
			free(s.str);
			free(str);
			s.exit_val = -2;
			continue ;
		}
		s.head_ll = s.l_lst;
		s.p_lst = s.head_parse;
		s.temp = s.head_ll;
		ft_free_parse_list(s.p_lst);
		err = parser(&s);
		err2 = parse_heredoc(&s);
		if (err2 == ERR_MALLOC || err == ERR_MALLOC)
		{
			ft_dprintf(STDERR_FILENO, "ERR_MALLOC");
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			s.exit_val = -2;
			continue ;
		}
		if (err == ERR_PARS || err2 == ERR_PARS)
		{
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			s.exit_val = 2;
			continue ;
		}
		if (err2 == QUIT)
		{
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			s.exit_val = 130;
			continue ;
		}
		// ft_print_list2(s.l_lst);
		s.tab = NULL;
		s.infile = NULL;
		s.outfile = NULL;
		s.mode_in = NOTHING;
		s.mode_out = NOTHING;
		s.token_in = NOTHING;
		s.token_out = NOTHING;
		s.count_pipes = 0;
		s.counter = 0;
		s.here_doc_open = 0;
		if (!check_list(s.head_ll))
		{
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			continue ;
		}
		count_pipes(&s);
		stat = execute(&s, s.head_ll, 0, 0);
		if (stat != EXIT && stat != NORMAL)
			s.exit_val = wait_pid(&s);
		if (stat == ERR_PARS)
		{
			ft_dprintf(2, "Malloc\n");
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			s.exit_val = -2;
			continue ;
		}
		else if (s.exit == EXIT)
		{
			ft_dprintf(2, "\033[1;95mexit\n\033[0m");
			if (s.bad_exit)
			{
				s.num_err_exit = 0;
				if (s.string_error)
				{
					free(s.string_error);
					s.string_error = NULL;
				}
				ft_dprintf(2, "%sMinishell: exit: too many arguments%s\n", PURP,
					RESET);
				ft_free_changed_list(s.l_lst);
				free(s.str);
				free(str);
				stat = 0;
				continue ;
			}
			if (s.num_err_exit)
			{
				ft_dprintf(2, "%s%s%s", ERR_X1, s.string_error, ERR_X2);
				free(s.string_error);
			}
			ft_free_changed_list(s.l_lst);
			free(s.str);
			free(str);
			rl_clear_history();
			free_env(s.env);
			free_env(s.dup_env);
			if (s.exit_arg && !s.bad_exit)
				exit(s.exit_arg);
			if (!s.bad_exit)
				exit(s.exit_val);
		}
		ft_free_changed_list(s.l_lst);
		free(s.str);
		free(str);
	}
	return (s.exit_val);
}
