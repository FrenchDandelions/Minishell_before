/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thole <thole@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 12:44:35 by thole             #+#    #+#             */
/*   Updated: 2024/02/04 12:44:37 by thole            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include "./libft/libft.h"
# define SUCCESS -3
# define ERR_MALLOC -2
# define TK_SKIP 0
# define TK_ARG 1
# define TK_PRIO 2
# define TK_WILD 3
# define TK_SINGLE 4
# define TK_DOUBLE 5
# define TK_INPUT 6
# define TK_OUTPUT 7
# define TK_DLMTR 8
# define TK_APPEND 9
# define TK_PIPES 10
# define TK_EXPAND 11
# define TK_AND 12
# define TK_OR 13
# define TK_ENV_VAR 14
# define BUILTIN 16
# define NOTHING 17
# define TK_END_PRIO 18
# define TK_END 19
# define ERR_PARS -18
# define ERR_LEFT_ANGLE -100
# define ERR_RIGHT_ANGLE -99
# define ERR_PIPE -98
# define ERR_AND -97
# define ERR_SINGLE -56
# define ERR_DOUBLE -55
# define TEMP_AND 20
# define SIZE 4096
# define IS_TTY 21
# define FAILURE -1000000
# define ERR_MINI_DOC "\033[0;32mMinishell: warning: mini_doc delimited by end-of-file (wanted `"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

typedef struct s_parse_list
{
	char				c;
	int					token;
	struct s_parse_list	*next;
	struct s_parse_list	*prev;
}						t_parse_list;

typedef struct s_last_list
{
	char				*str;
	int					token;
	struct s_last_list	*next;
	struct s_last_list	*prev;
}						t_last_list;

typedef struct s_file
{
	char				*infile;
	char				*outfile;
	int					mode_in;
	int					mode_out;

}						t_file;

typedef struct s_tree
{
	t_file				*file;
	char				**cmd_tab;
	char				*infile;
	char				*outfile;
	int					err_code;
	struct s_tree		*right;
	struct s_tree		*left;
}						t_tree;

typedef struct s_struct
{
	char				*str;
	int					err;
	char				*tab[4096];
	char				*file_in;
	char				*file_out;
	int					mode_in;
	int					mode_out;
	char				*delim;
	t_parse_list		*p_lst;
	t_last_list			*l_lst;
	t_last_list			*head_ll;
	t_parse_list		*head_parse;
	t_last_list			*temp;
}						t_struct;

int						ft_prototype_list(t_struct *s);
void					ft_free_parse_list(t_parse_list *list);
void					*ft_memcpy(void *dest, const void *src, size_t n);
int						ft_change_list(t_struct *parse);
void					ft_free_changed_list(t_last_list *list);
int						parser(t_struct *s);
// char					*get_next_line(int fd);
int						open_heredoc(t_last_list **list);
char					*ft_gnl_strjoin(char *s1, char *s2, size_t len);
int						parse_heredoc(t_struct *s);
int						quote_checker(char *str);
void					ft_free_changed_list(t_last_list *list);
int						execute(t_struct *s, t_last_list *list);

#endif
