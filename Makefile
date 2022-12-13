NAME = minishell
LIBFTNAME = libft.a
LIBFTPATH = ./libft

SRC_DIR = ./src/
SRC = parsing/my_parser.c parsing/my_shell.c parsing/utils_lib.c parsing/pre_processor.c \
	  parsing/ft_extract_tokens.c parsing/ft_create_parsed_cmd_list.c

OBJS = ${addprefix ${SRC_DIR}, ${SRC:.c=.o}}

# /* ~~~~~~~ INCLUDING GNL ~~~~~~~ */
GNL_DIR = ./gnl/
GNL = get_next_line.c \
	get_next_line_utils.c \

GNL_OBJS = ${addprefix ${GNL_DIR}, ${GNL:.c=.o}}


FLAGS = -Wall -Wextra -Werror -g
LINKS = -lreadline

NONE='\033[0m'
GREEN='\033[32m'
GRAY='\033[2;37m'
CURSIVE='\033[3m'

all: $(NAME)

$(NAME): $(OBJS) $(GNL_OBJS)
	@echo $(CURSIVE)$(GRAY) "     - Compiling $(NAME)..." $(NONE)
	@make -C ${LIBFTPATH}
	@gcc $(OBJS) $(GNL_OBJS) ./libft/libft.a $(LINKS) -o $(NAME)
	@echo $(GREEN)"- Compiled -"$(NONE)

%.o			: %.c
				gcc -c $< -o ${<:.c=.o}
clean:
	@echo $(CURSIVE)$(GRAY) "     - Removing object files..." $(NONE)
	@rm -rf $(OBJS)
	@rm -rf $(GNL_OBJS)

fclean: clean
	@echo $(CURSIVE)$(GRAY) "     - Removing $(NAME)..." $(NONE)
	@rm -rf $(NAME)

re: fclean all
