# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 00:56:29 by ggalon            #+#    #+#              #
#    Updated: 2024/01/28 00:56:29 by ggalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# DIRECTORIES ==================================================================

LIBFT_DIR	=	libft/

SRCS_DIR	=	src/

OBJS_DIR	=	obj/

INCL_DIR	=	inc/

# FILES ========================================================================

NAME	=	minishell

LIBFT	=	$(LIBFT_DIR)libft.a

FILE_C	=	minishell signal \
			lexer/lexer lexer/token lexer/utils_1 lexer/utils_2 \
			expander/expander expander/env expander/utils \
			execution/execution \
			builtins/echo builtins/cd builtins/pwd builtins/export builtins/unset builtins/env builtins/exit builtins/utils \

FILE_H	=	minishell

SRCS	=	$(addsuffix .c, $(addprefix $(SRCS_DIR), $(FILE_C)))

OBJS	=	$(addsuffix .o, $(addprefix $(OBJS_DIR), $(FILE_C)))

INCL	=	$(addsuffix .h, $(addprefix $(INCL_DIR), $(FILE_H)))

# COMMANDS =====================================================================

CC			=	gcc

CC_FLAGS	=	-Wall -Wextra -Werror

AR			=	ar

AR_FLAGS	=	-rc

NORM		=	norminette.exe $(SRCS_DIR) $(INCL_DIR)

# RULES ========================================================================

all:
	@ echo "\n${BIBlue}Checking Norminette...${NC}"
	@ $(NORM) | grep -q Error && $(NORM) | grep Error || echo "\n${BIGreen}Norminette OK !${NC}"
	@ $(MAKE) --no-print-directory -C $(LIBFT_DIR)
	@ mkdir -p $(OBJS_DIR) $(OBJS_DIR)/lexer $(OBJS_DIR)/expander $(OBJS_DIR)/execution $(OBJS_DIR)/builtins
	@ echo "\n${BIBlue}Compilation of project source files...${NC}"
	@ $(MAKE) --no-print-directory $(NAME)
	@ echo "\n${BIGreen}Project Ready !${NC}\n"

$(NAME): $(OBJS)
	@ echo "\n${BICyan}Creating the executable...${NC}"
	$(CC) $(CC_FLAGS) $(OBJS) $(LIBFT) -o $(NAME) -L/usr/local/lib -lreadline

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(INCL) Makefile
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	@ $(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@ echo "\n${BIRed}Project binary deletion...${NC}"
	rm -rf $(OBJS_DIR)

fclean:
	@ $(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@ echo "\n${BIRed}Project deletion...${NC}"
	rm -rf $(OBJS_DIR)
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY: all clean fclean re bonus

# COLORS =======================================================================

# Reset
NC=\033[0m

# Regular Colors
Black=\033[0;30m
Red=\033[0;31m
Green=\033[0;32m
Yellow=\033[0;33m
Blue=\033[0;34m
Purple=\033[0;35m
Cyan=\033[0;36m
White=\033[0;37m

# Bold
BBlack=\033[1;30m
BRed=\033[1;31m
BGreen=\033[1;32m
BYellow=\033[1;33m
BBlue=\033[1;34m
BPurple=\033[1;35m
BCyan=\033[1;36m
BWhite=\033[1;37m

# Underline
UBlack=\033[4;30m
URed=\033[4;31m
UGreen=\033[4;32m
UYellow=\033[4;33m
UBlue=\033[4;34m
UPurple=\033[4;35m
UCyan=\033[4;36m
UWhite=\033[4;37m

# Background
On_Black=\033[40m
On_Red=\033[41m
On_Green=\033[42m
On_Yellow=\033[43m
On_Blue=\033[44m
On_Purple=\033[45m
On_Cyan=\033[46m
On_White=\033[47m

# High Intensity
IBlack=\033[0;90m
IRed=\033[0;91m
IGreen=\033[0;92m
IYellow=\033[0;93m
IBlue=\033[0;94m
IPurple=\033[0;95m
ICyan=\033[0;96m
IWhite=\033[0;97m

# Bold High Intensity
BIBlack=\033[1;90m
BIRed=\033[1;91m
BIGreen=\033[1;92m
BIYellow=\033[1;93m
BIBlue=\033[1;94m
BIPurple=\033[1;95m
BICyan=\033[1;96m
BIWhite=\033[1;97m

# High Intensity backgrounds
On_IBlack=\033[0;100m
On_IRed=\033[0;101m
On_IGreen=\033[0;102m
On_IYellow=\033[0;103m
On_IBlue=\033[0;104m
On_IPurple=\033[0;105m
On_ICyan=\033[0;106m
On_IWhite=\033[0;107m