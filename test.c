

// void	free_parse(void)
// {
// 	ft_free_parse_list(s()->p.s.p_lst);
// 	ft_free_changed_list(s()->p.s.l_lst);
// 	free(s()->e.line);
// 	free(s()->p.s.line);
// }

// int	error_checkers(char *str)
// {
// 	dprintf(2, "%s", str);
// 	free(s()->e.line);
// 	return (CONTINUE);
// }
// int	free_and_send_err(char *str)
// {
// 	ft_free_parse_list(s()->p.s.p_lst);
// 	return (error_checkers(str));
// }

// int	set_heads_and_parse(void)
// {
// 	s()->p.s.head_ll = s()->p.s.l_lst;
// 	s()->p.s.p_lst = s()->p.s.head_parse;
// 	s()->p.s.temp = s()->p.s.head_ll;
// 	s()->p.s.err3 = parser(&s);
// 	if (s()->p.s.err3 != ERR_MALLOC && s()->p.s.err3 != ERR_PARS)
// 		s()->p.s.err2 = parse_heredoc(&s);
// 	if (s()->p.s.err2 == ERR_MALLOC || s()->p.s.err3 == ERR_MALLOC)
// 	{
// 		dprintf(STDERR_FILENO, "ERR_MALLOC");
// 		free_parse();
// 		return (CONTINUE);
// 	}
// 	if (s()->p.s.err3 == ERR_PARS || s()->p.s.err2 == ERR_PARS)
// 	{
// 		free_parse();
// 		return (CONTINUE);
// 	}
// 	s()->p.s.tab[0] = NULL;
// 	return (SUCCESS);
// }

// int	process(void)
// {
// 	s()->p.s.stat = execute(&s, s.head_ll, 0, 0);
// 	if (s()->p.s.stat == ERR_PARS)
// 	{
// 		dprintf(2, "Malloc\n");
// 		free_parse();
// 		return (CONTINUE);
// 	}
// 	else if (s()->p.s.stat == EXIT)
// 	{
// 		dprintf(2, "exit\n");
// 		free_parse();
// 		return (BREAK);
// 	}
// 	free_parse();
// 	return (SUCCESS);
// }

// int	ft_parsing(void)
// {
// 	if (quote_checker(s()->e.line) < SUCCESS)
// 		return (error_checkers("Minishell: error: unclosed quotes\n"));
// 	if (check_parenthesis(s()->e.line) == ERR_PARS)
// 		return (error_checkers("Minishell: error: unclosed parenthesis\n"));
// 	s()->p.s.line = strdup(s()->e.line);
// 	s()->p.s.err = 0;
// 	if (ft_prototype_list(&s()->p.s) == ERR_MALLOC)
// 		return (error_checkers("Malloc failed"));
// 	s()->p.s.head_parse = s()->p.s.p_lst;
// 	if (ft_change_list(&s()->p.s) == ERR_MALLOC)
// 		return ();
// 	s()->p.s.global_err = set_heads_and_parse();
// 	if (s()->p.s.global_err != SUCCESS)
// 		return (s()->p.s.global_err);
// 	return (process());
// }

// void	ft_minishell(void)
// {
// 	printf("%s\n", ft_get_shell()->argv[0]);
// 	// ft_buildin_load();
// 	s()->tabenv = ft_get_envp(s()->env);
// 	s()->e.pwd = s()->tabenv[ENVP_PWD];
// 	s()->e.env = ft_splitcpy(s()->env);
// 	if (s()->tabenv)
// 	{
// 		while (!s()->e.exit)
// 		{
// 			s()->e.line = readline("\033[1;34mMinishell$\033[0m ");
// 			if (!s()->e.line)
// 			{
// 				rl_clear_history();
// 				printf("Bye ;)\n");
// 				break ;
// 			}
// 			add_history(s()->e.line);
// 			s()->p.s.status = ft_parsing();
// 			if (s()->p.s.status == CONTINUE)
// 				continue ;
// 			else
// 				break ;
// 		}
// 	}
// 	ft_free_shell();
// }
