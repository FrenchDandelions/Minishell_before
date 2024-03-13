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
# define EXIT 777
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
# define SIZE 16384
# define IS_TTY 21
# define FAILURE -1000000
# define WINNING 1000000
# define ERR_FORK -200
# define ERR_MINI_DOC "\033[0;32mMinishell: warning: mini_doc delimited by end-of-file (wanted `"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <malloc.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

void					sig_int(int code);
void					sig_quit(int code);
void					sig_init(void);

extern int				g_sig;

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
	char				*files[SIZE];
	char				*here_doc_file;
	int					modes[SIZE];
	int					token[SIZE];
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
	char				*tab[SIZE];
	char				*infile;
	char				*outfile;
	int					mode_in;
	int					mode_out;
	int					token_in;
	int					token_out;
	int					exit;
	char				*here_doc_file;
	char				*delim;
	char				**env;
	int					pipe[2];
	int					is_pipe;
	int					here_doc[2];
	pid_t				pid;
	t_parse_list		*p_lst;
	t_last_list			*l_lst;
	t_last_list			*head_ll;
	t_parse_list		*head_parse;
	t_last_list			*temp;
	char				**dup_env;
	int					end;
	int					fd_in;
	int					fd_out;
	int					here_doc_open;
	int					is_first;
	int					is_last;
	int					last_fd;
	int					count_pipes;
	int					counter;
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
int						execute(t_struct *s, t_last_list *list, int depth,
							int pipe);
void					ft_print_list2(t_last_list *lst);
t_last_list				*new_list(void);
int						go_to_next_stop(t_last_list **list);
char					*takeoff_quotes(char *str);
char					*get_path(char *cmd, char **env, int *flag);
int						exec(t_struct *s, t_file *file);
void					ft_echo(t_struct *s);
void					ft_pwd(t_struct *s);
void					ft_cd(t_struct *s);
void					ft_export(t_struct *s, char **env);
void					ft_unset(t_struct *s, char **env);
void					ft_env(t_struct *s, char **env, int fake_env);
void					ft_exit(t_struct *s);
int						do_files(t_file *f, t_struct *s);
void					free_tab(char **tab);
void					free_array(char **array);
int						token_redirection(int token, int mode);
int						open_output(char *s, int token, t_struct *st);
int						open_append(char *s, int token, t_struct *st);
int						open_input(char *s, int token, t_struct *st);
int						open_dlmtr(char *s, int token, t_struct *st);
char					*takeoff_quotes(char *str);
void					count_pipes(t_struct *s);
char					*ft_strnjoin(char *s1, char *s2, size_t size_s1,
							int start);

#endif
