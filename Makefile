# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 14:31:53 by aheitz            #+#    #+#              #
#    Updated: 2025/08/13 07:15:10 by aheitz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME     = ft_shmup
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++2b -MMD -MP

LIBS = -lncurses

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRC = $(SRC_DIR)/main.cpp                   \
	$(SRC_DIR)/gameplay/gameplay.cpp        \
	$(SRC_DIR)/render/render.cpp            \
	$(SRC_DIR)/gameplay/obstacle.cpp        \
	$(SRC_DIR)/gameplay/enemy/shooter.cpp   \
	$(SRC_DIR)/gameplay/enemy/bomber.cpp    \
	$(SRC_DIR)/gameplay/enemy/dodger.cpp

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)