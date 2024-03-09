int	do_exec(t_struct **s, t_file *file, int stat)
{
	int i;
	int j;
	int rd;

	if (stat)
	{
		rd = rand() % 2;
		if (rd == 0)
		{
			if (stat == 2)
				(*s)->err = WINNING;
			else
				(*s)->err = FAILURE;
		}
		else
		{
			if (stat == 1)
				(*s)->err = WINNING;
			else
				(*s)->err = FAILURE;
		}
	}
	i = 0;
	j = 0;
	(void)stat;
	if ((*s)->tab[i])
	{
		if (strcmp((*s)->tab[i], "exit") == 0)
			return ((*s)->exit = EXIT, EXIT);
	}
	if ((*s)->tab[i])
		printf("Command : %s\n", (*s)->tab[i]);
	i++;
	if ((*s)->tab[i - 1])
	{
		while ((*s)->tab[i])
		{
			printf("Command : %s\n", (*s)->tab[i]);
			i++;
		}
	}
	i = 0;
	while (file->files[i])
	{
		if (token_redirection(file->modes[i], 1) == 2)
		{
			(*s)->infile = file->files[i];
			(*s)->token_in = file->token[i];
			(*s)->mode_in = file->modes[i];
		}
		else if (token_redirection(file->modes[i], 1) == 1)
		{
			(*s)->outfile = file->files[i];
			(*s)->token_out = file->token[i];
			(*s)->mode_out = file->modes[i];
		}
		i++;
	}
	if (i > 0)
		i--;
	if (j > 0)
		j--;
	printf("Infile : %s, %s : %d, %s : %d, Outfile : %s, %s : %d, %s : %d\n",
		(*s)->infile, "Mode Infile", (*s)->mode_in, "Token Infile",
		(*s)->token_in, (*s)->outfile, "Mode Outfile", (*s)->mode_out,
		"Token Outfile", (*s)->token_out);
	return (SUCCESS);
}