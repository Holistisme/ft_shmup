OBJS_DIR	= objects/
SRCS_DIR 	= srcs/

#Sources
SRC			= mainb.cpp

SRCS		= $(addprefix ${SRCS_DIR}, ${SRC})

#Object
OBJS		= $(addprefix ${OBJS_DIR}, ${SRC:.cpp=.o})

INCLUDES	= includes
NAME		= ft_shmup
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror -I${INCLUDES} -g3
CC			= c++

#Colors
LIGHT_GRAY	= \033[2m
ORANGE		= \033[1;33m
DARK_GRAY	= \033[0;90m
RED			= \033[0;91m
GREEN		= \033[0;92m
YELLOW		= \033[0;93m
BLUE		= \033[0;94m
MAGENTA		= \033[0;95m
CYAN		= \033[0;96m
WHITE		= \033[0;97m
RESET		= \033[0m

#Forme
BOLD		= \033[1m
ITALIC		= \033[3m
UNDERLINE	= \033[4m
CROSS		= \033[9m
FLASH		= \033[5m
NEGATIF		= \033[7m

all:			${NAME}

${OBJS_DIR}%.o: ${SRCS_DIR}%.cpp | ${OBJS_DIR}
				@${CC} ${CFLAGS} -c $< -o $@

${NAME}:		${OBJS}
				@${CC} ${CFLAGS} ${OBJS} -lncurses -o $@ 
				@echo "${YELLOW}'$@' is compiled ! ✅${RESET}"

${OBJS_DIR}:
				@mkdir -p ${OBJS_DIR}

clean:
				@${RM} ${OBJS}
				@${RM} -r ${OBJS_DIR}
				@echo "${RED}'${NAME}' objects are deleted ! 👍${RESET}"

fclean:			clean
				@${RM} ${NAME}
				@echo "${RED}'${NAME}' is deleted ! 👍${RESET}"

re:				fclean all

.PHONY:			all clean fclean re
