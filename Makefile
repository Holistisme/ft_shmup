# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 14:31:53 by aheitz            #+#    #+#              #
#    Updated: 2025/08/13 13:30:54 by aheitz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     = ft_shmup
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++2b -MMD -MP

LIBS = -lncurses

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
SRC_DIR_BONUS = bonus/src

SRC = $(SRC_DIR)/main.cpp                   \
	$(SRC_DIR)/gameplay/gameplay.cpp        \
	$(SRC_DIR)/render/render.cpp            \
	$(SRC_DIR)/gameplay/obstacle.cpp        \
	$(SRC_DIR)/gameplay/enemy/shooter.cpp   \
	$(SRC_DIR)/gameplay/enemy/bomber.cpp    \
	$(SRC_DIR)/gameplay/enemy/dodger.cpp    \

SRC_BONUS	= $(SRC_DIR)/$(SRC_DIR_BONUS)/main.cpp            \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/gameplay.cpp \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/render/render.cpp     \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/obstacle.cpp \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/enemy/shooter.cpp  \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/enemy/bomber.cpp   \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/enemy/dodger.cpp   \
			  $(SRC_DIR)/$(SRC_DIR_BONUS)/gameplay/enemy/boss.cpp     \

OBJ_BONUS = $(SRC_BONUS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

bonus: $(OBJ_BONUS)
	$(CXX) $(CXXFLAGS) -o $(NAME)_bonus $^ $(LIBS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME) $(NAME)_bonus

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)
