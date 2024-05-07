/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 09:07:26 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/08 15:54:25 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include "../libft/libft.h"
# include <sys/types.h>
# include <dirent.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/stat.h>

/*Color Definition*/
# define RED     "\x1b[31;1m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"

/*Fds*/
# define FD_READ 0
# define FD_WRITE 1
# define FD_IN 0
# define FD_OUT 1

/**MESSAGE DEFINITION**/
# define ARGS_ERR_MSG "Error : Invalid number of arguments.\n"
# define UNAVAILABLE_ENV "Minishell: Environment unavailable.\n"
# define MALLOC_ERR_MSG "Minishell: Allocation error\n"
# define QUOTES_ERR_MSG "Minishell: open quotes are not interpreted.\n"
# define INFILE_ERR_MSG "Minishell: open quotes are not interpreted.\n"
# define PARENTHESIS_ERR_MSG "Minishell: \
open parenthesis are not interpreted.\n"
# define OPEN_PIPE_ERR_MSG "Minishell: open pipe are not interpreted.\n"
# define OPEN_LOGICAL_OP_ERR_MSG "Minishell: \
open logical operators are not interpreted.\n"
# define WRONG_CHAR_ERR_MSG "Minishell: uninterpreted token \
present outside quotes.\n"
# define NEAR_TOKEN_ERR_MSG "Minishell: syntax error near unexpected token"
# define STAR_TOKEN_ERR_MSG "Minishell: *: ambiguous redirect\n"
# define UNEXPECTED_EOF "Minishell: syntax error: unexpected end of file.\n"
# define EOF_HEREDOC "Minishell:\
 warning: here-document at line 1 delimited by end-of-file"
# define PWD_ERR_MSG "pwd: error retrieving current directory: getcwd: \
cannot access parent directories: No such file or directory"
# define SHLVL_ERR_MSG "minishell: warning: shell level (%d) too high, \
resetting to 1"
# define HEREDOC_MSG "%sMinishell: warning: here-document \
at line %d delimited by end-of-file (wanted `%s')%s\n"
# define DOT_MSG_ERR ": filename argument required\n.: \
usage: . filename [arguments]\n"

extern int	g_status;

typedef enum e_id_gc
{
	TKN_LIST = 1,
	B_TREE,
	ENV,
	TMP
}	t_id_gc;

typedef enum e_tkntype
{
	WORD,
	IN,
	OUT,
	HEREDOC,
	APPEND,
	PIPE,
	AND,
	OR,
	LEFT_PAR,
	ONE_QUOTE,
	TWO_QUOTE,
	PARENTHESIS,
}	t_tkntype;

typedef struct s_io
{
	int	fd_in;
	int	fd_out;
}	t_io;

typedef struct s_btree
{
	char			**cmds;
	int				branch;
	t_tkntype		type;
	struct s_btree	*left;
	struct s_btree	*right;
}	t_btree;

typedef enum e_error
{
	NOTHING = -1,
	MALLOC_E,
	FILE_E,
	TYPE_E
}	t_error;

typedef struct s_token
{
	t_tkntype		type;
	char			*content;
	int				link;
	size_t			index;
	int				priority;
	int				used_flag;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s__tknlist
{
	struct s_token	*head;
	struct s_token	*tail;
}	t_tknlist;

typedef struct s_env
{
	char			*value;
	int				secret;
	struct s_env	*next;
}				t_env;

typedef struct s_mini
{
	t_env			*env;
	t_tknlist		*tkn_lst;
	t_btree			*b_tree;
	t_io			io_global;
	int				last_gstatus;
}				t_mini;

/**Functions**/
t_token		*create_node(t_tkntype typed, char *value, int linked);
int			add_node(t_tknlist *list, t_token *node);
void		init_list(t_tknlist **list);
t_tknlist	*lexer(char *buffer);
int			detect_type(const char c, const char c2);
void		handle_token(char *buffer, t_tknlist *list, t_tkntype type, int *i);
char		*ft_strndup(char *buffer, int len, t_id_gc id);
int			is_operator(const char c, const char c2);
int			detect_error_type(const char c);
int			simple_quote_handler(char *buffer, t_tknlist *list);
int			pipe_handler(char *buffer, t_tknlist *list);
int			file_handler(char *buffer, t_tknlist *list, t_tkntype type);
int			cmd_handler(char *buffer, t_tknlist *list);
int			parenthese_handler(char *buffer, t_tknlist *list);
int			double_quote_handler(char *buffer, t_tknlist *list);
int			operator_handler(char *buffer, t_tknlist *list, t_tkntype type);
int			error_handler_lexer(int id_gc, char *msg);
int			is_in_env(t_env *env, char *args);
int			update_oldpwd(t_env **env);
int			update_pwd(t_env **env, int slash);
int			go_to_path(t_env **env);
int			cd(char **cmds, t_env **env);
int			echo(char **cmds, t_io fds);
int			size_cmds(char **cmds);
int			env(t_env *env, t_io fds);
int			ft_pwd(t_io fds, t_env *env);
int			ft_export(char **args, t_env **envt, t_io fds);
int			unset(char **value, t_env **env);
int			builtin_exit(t_mini *mini, char **cmds);
t_env		*init_env(char **env_array);
char		*ft_strndup(char *buffer, int len, t_id_gc id);
void		env_add_back(t_env **env, t_env *new);
int			env_add(char *value, t_env **env, int mod);
void		*get_env_name_var(char *dest, char *src);
size_t		size_all_value(t_env *lst);
char		*env_to_str(t_env *lst);
char		*get_env_value(t_env *env, const char *var, size_t len);
int			strlen_2d(char **tab);
void		sort_env(char **tab, int len_env);
void		print_sorted_env(t_env *env, t_io fds);
char		*create_prompt(t_mini *mini);
void		prompt_loop(t_mini *mini);
void		add_after_another(t_tknlist	*list, \
t_token *el_flag, t_token *el_toplace);
int			is_logical_op_tkn(t_tkntype tkntype);
int			is_cmd_tkn(t_tkntype tkntype);
int			is_redir_tkn(t_tkntype tkntype);
int			is_operator_tkn(t_tkntype tkntype);
void		pop_token_in_place(t_tknlist *list_tkn, t_token *to_pop);
void		add_tknlst_in_tknlst_after_target(t_tknlist *list,
				t_token *target_tkn, t_tknlist *list_expnd);
void		swap_tokens(t_tknlist	*lst, t_token *tkn1, t_token *tkn2);
size_t		tknlst_size(t_tknlist *tknlst);
void		display_tknlist(t_tknlist *list);
char		*strcut_gc(char const *str,
				size_t cut_begin, size_t cut_end, int id_gc);
char		*ft_strcut(char const *str, size_t cut_begin, size_t cut_end);
char		*replace_substr(char *str,
				char *replacement, size_t start, size_t len);
char		*remove_substr(char *str, size_t start, size_t len);
size_t		ft_strlen_until(const char *str, int (*f)(char));
size_t		ft_strlen_until_not(const char *str, int (*f)(char));
size_t		ft_strlen_until_char(const char *str, char c);
size_t		ft_strlen_until_not_char(const char *str, char c);
int			str_contains_all_subs_ordered(char *str, char **subs);
int			char_is_in_str(char c, char *str);
int			ft_strcmp_case_insensitive(char *s1, char *s2);
int			s1_is_s2_suffix(char *s1, char *s2);
bool		s1_is_s2_prefix(char *s1, char *s2);
t_btree		*parser(t_mini *mini);
void		verify_syntax_tknlist(t_tknlist *lst);
int			type_need_reducing(t_tkntype type);
void		reducer(t_token	*tkn);
int			is_link_sensitive(t_tkntype type);
void		linker(t_tknlist *tkn_lst);
void		expander(t_mini *mini, t_tknlist *tkn_lst);
t_token		*expand_dollar(t_mini *mini,
				t_token *tkn_toexpand, t_tknlist *tkn_lst);
t_token		*expand_wildcard(t_token *tkn_toexpand, t_tknlist *tkn_lst);
t_btree		*place_in_tree(t_btree *tree_el, t_btree *toplace, int index);
t_btree		*btree_new(t_token	*tkn_toconvert);
t_btree		*create_bin_tree(t_tknlist *tknlst);
size_t		count_argc_cmd(t_token *curr);
char		**get_argv_cmd(t_token *curr, size_t argc);
char		**strdup_in_newchar2(char *delimiter);
char		**extract_cmd_argv(t_token *curr);
t_token		*find_prior_token(t_token *curr);
int			cmd_is_inside_pipe(t_btree *root, int branch_id);
void		depth_first_search(t_btree *tree_el, void (*visit)(t_btree *, int));
void		display_node(t_btree *tree_el, int depth);
void		root_first_search(t_btree *tree_el, void (*visit)(t_btree *, int));
int			fork_builtin(t_env **envt, t_btree *tree_el, t_io fds);
int			exec_builtin(t_env **envt, t_btree *tree_el, t_io fds);
int			is_builtin(char *command);
int			exec_handler(t_mini *mini, t_btree *tree_el, t_io fds);
int			exec_bin(t_env *env, t_btree *tree_el, t_io fds);
void		exec_process(t_btree *tree_el, t_env *env, t_io fds);
char		*get_cmd_path(char *cmd, t_env *env);
char		**find_path(t_env *env);
char		*check_command_path(char *cmd, char *path);
char		*ft_strjoin_pipex(char *s1, char *s2, char *sep);
char		**env_to_char2(t_env *env);
void		traverse_heredoc_node(t_mini *mini,
				t_btree *tree_el, t_io io_inherited);
void		traverse_redir_input_node(t_mini *mini,
				t_btree *tree_el, t_io io_inherited);
void		traverse_redir_output_node(t_mini *mini,
				t_btree *tree_el, t_io io_inherited);
void		traverse_pipe_node(t_mini *mini, t_btree *tree_el,
				t_io io_inherited);
void		traverse_logical_op_node(t_mini *mini,
				t_btree *tree_el, t_io io_inherited);
void		browse_tree(t_mini *mini, t_btree *tree_el, t_io io_inherited);
t_mini		*singleton_mini(t_mini *address_mini);
void		print_and_exit(char *msg, char *color, int exit_status);
void		print_without_exit(char *msg, char *color, int exit_status);
void		print_strerror(char *arg, char *color, int new_status);
void		free_and_exit(int exit_code);
void		rearrange_cmd_redir_order(t_tknlist *tknlst);
char		*path_handler(t_btree *tree_el, t_env *env);
char		*ft_strjoin_pipex(char *s1, char *s2, char *sep);
void		check_path(char *command, char *path);
char		*check_command_path(char *cmd, char *path);
char		**find_path(t_env *env);
char		*get_cmd_path(char *cmd, t_env *env);
size_t		strlen_env(t_env *env);
void		print_path_error(char *arg, int exit_status, int error);
int			is_logical_op_tkn(t_tkntype tkntype);
int			is_cmd_tkn(t_tkntype tkntype);
int			is_redir_tkn(t_tkntype tkntype);
int			is_operator_tkn(t_tkntype tkntype);
void		clear_loop(void);
int			wait_child(pid_t pid);
int			traverse_parenthesis_node(t_mini *mini,
				t_btree *tree_el, t_io io_inherited);
void		unbound_token_in_place(t_tknlist *list_tkn, t_token *to_pop);
void		tknlst_addfront(t_token *curr, t_tknlist *tknlst);
void		tknlst_addback(t_token *curr, t_tknlist *tknlst);
t_token		*return_end_sequence(t_token *begin_seq);
size_t		size_sequence(t_token *begin_seq);

#endif