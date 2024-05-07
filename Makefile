.DEFAULT_GOAL : $(NAME)

NAME = minishell

NAME_BONUS = minishell_bonus

CC = cc

CFLAGS = -Werror -Wall -Wextra

RM = rm -rf

SRCS =  srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/exit.c \
		srcs/builtins/export.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c \
		\
		srcs/env/env_handler.c \
		srcs/env/env_utils.c \
		srcs/env/sort_env.c \
		\
		srcs/exec/browse_node_heredoc.c\
		srcs/exec/browse_node.c \
		srcs/exec/browse_tree.c \
		srcs/exec/exec_bin.c \
		srcs/exec/exec_builtins.c \
		srcs/exec/exec_handler.c \
		srcs/exec/get_path.c \
		\
		srcs/lexer/error_handler.c \
		srcs/lexer/lexer_utils.c \
		srcs/lexer/lexer.c \
		srcs/lexer/token_handler_suite.c \
		srcs/lexer/token_handler.c \
		\
		srcs/libft_extension/libft_extension_1.c \
		srcs/libft_extension/libft_extension_2.c \
		srcs/libft_extension/libft_extension_3.c \
		\
		srcs/parser/binary_tree/bin_tree_utils.c \
		srcs/parser/binary_tree/bin_tree.c \
		\
		srcs/parser/expander/expand_dollars.c \
		srcs/parser/expander/expand_wildcards.c \
		srcs/parser/expander/expander.c \
		\
		srcs/parser/linker/linker.c \
		\
		srcs/parser/reducer/reducer.c \
		\
		srcs/parser/tknlist_handler/reorder_tknlist.c \
		srcs/parser/tknlist_handler/reorder_tknlist2.c \
		\
		srcs/parser/parser.c \
		srcs/parser/verify_syntax.c \
		\
		srcs/terminal/loop.c \
		srcs/terminal/prompt.c \
		\
		srcs/utils/error.c \
		srcs/utils/print_utils.c \
		srcs/utils/tknlist_utils_2.c \
		srcs/utils/tknlist_utils_3.c \
		srcs/utils/tknlist_utils.c \
		srcs/utils/tkntype_utils.c \
		\
		srcs/main.c \


OBJS = ${SRCS:.c=.o}

LIBFT_DIR = libft
LIBFT_A = libft/libft.a
_CLEAR	= \033[0K\r\c
_OK	= [\033[32mOK\033[0m]

all : force $(NAME)

$(LIBFT_A): libft/libft.h
	@echo "[..] libft... compiling \r\c"
	@$(MAKE) -C $(LIBFT_DIR) -s
	@echo "$(_CLEAR)"

${NAME}: ${OBJS} $(LIBFT_A)
	@${CC} ${CFLAGS} -g3 -I include -I libft/ -o $@ $^ -Llibft -lft -lreadline
	@echo "$(_OK) $(NAME) compiled"

${NAME_BONUS}: ${OBJS} $(LIBFT_A)
	@${CC} ${CFLAGS} -g3 -I include -I libft/ -o $@ $^ -Llibft -lft -lreadline
	@echo "$(_OK) $(NAME_BONUS) compiled"

%.o: %.c  include/minishell.h libft/libft.h
	@echo "[..] $(NAME_SHORT)... compiling $*.c\r\c"
	@${CC} ${CFLAGS} -g3 -Iinclude -Ilibft/ -c $< -o $@
	@echo "$(_CLEAR)"

force :
	make -C libft

clean :
	@$(MAKE) -C $(LIBFT_DIR) clean -s
	@$(RM) $(OBJS)
	@$(RM) $(OBJS_BONUS)

fclean : clean
	@$(MAKE) -C $(LIBFT_DIR) fclean -s
	@$(RM) $(NAME)
	@$(RM) $(NAME_BONUS)

re : fclean all

bonus : force $(NAME_BONUS)

.PHONY: all clean fclean re force bonus

