# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/11 14:31:53 by aheitz            #+#    #+#              #
#    Updated: 2025/08/13 12:30:27 by vsyutkin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ############################################################################ #
#						# PROJECT SOURCES DECLARATIONS #			   #
# ############################################################################ #

#Only .c files
MAIN =	$(MANDATORY)miniRT.c
MAIN_BONUS =	$(BONUS)./miniRT_bonus.c

CHECKS_FILES =	checker.c \
		checker_cy.c \
		checker_line.c \
		checker_milieu.c \
		checker_sp_pl.c \
		check_is_angle.c \
		check_is_color.c \
		check_is_light_ratio.c \
		check_is_real.c \
		check_is_size_param.c \
		check_is_vector.c \
		check_is_xyz_c.c \
		check_utils.c \
		ft_strtof.c \

CHECKS_BONUS =	checker_cone.c \

FREES_FILES =	display_error.c \
		ft_check_error.c \
		ft_parser_error.c \
		ft_render_error.c \
		free_lists.c \
		malloc_err_chk.c \

PARSING_FILES = ft_str_to_color.c \
		ft_strtof_prs.c \
		ft_str_to_xyz.c \
		parser.c \
		parser_cylinder.c \
		parser_milieu.c \
		parser_plane.c \
		parser_sphere.c \

PARSING_BONUS = parser_cone.c \

MATH_FILES = 	vector_functions.c \
		vector_functions_2.c \
		quaternion_functions.c \
		quaternion_functions_2.c \
		quaternion_functions_3.c \

RENDERING_FILES =	compute_pixel_color.c \
			render_setup.c \
			render.c \
			setup_view_port.c \
			setup_quaternion.c \
			setup_camera_ray.c \
			find_object_intersection.c \
			find_plane_intersection.c \
			find_sphere_intersection.c \
			find_cylinder_intersection.c \
			define_color_1.c \
			define_color_2.c \
			diffuse.c \

RENDERING_BONUS	=	find_cone_intersection.c \
			find_cone_intersection_2.c \
			specular.c \

DISPLAY_INTERACTION_FILES = ft_mlx.c \
							esc.c \
							key_help.c \
							key_hook.c \
							key_light_diffusion.c \
							key_render_on_off.c \
							key_scale_control.c \
							key_select_light.c \
							key_select_object.c \
							key_specular.c \
							camera_move/key_camera_hook.c \
							camera_move/key_camera_rotateX.c \
							camera_move/key_camera_rotateY.c \
							camera_move/key_camera_rotateZ.c \
							camera_move/key_camera_translateX.c \
							camera_move/key_camera_translateY.c \
							camera_move/key_camera_translateZ.c \
							light_move/key_light_hook.c \
							light_move/key_light_translateX.c \
							light_move/key_light_translateY.c \
							light_move/key_light_translateZ.c \
							object_move/key_object_hook.c \
							object_move/key_object_rotateX.c \
							object_move/key_object_rotateY.c \
							object_move/key_object_rotateZ.c \
							object_move/key_object_translateX.c \
							object_move/key_object_translateY.c \
							object_move/key_object_translateZ.c \
							object_transform/height.c \
							object_transform/radius.c \

# ############################################################################ #
#						# PROJECT PATH DECLARATIONS #				   #
# ############################################################################ #

MANDATORY =	./src/
BONUS =		./src_bonus/

#Only directories
INCLUDES_DIR =	./include/

PROJECT_DIR =		$(INCLUDES_DIR) \
					$(MANDATORY) \
					$(BONUS) \

GAMEPLAY_DIR =				$(MANDATORY)./gameplay/
RENDER =					$(MANDATORY)./render/
ENEMY_DIR = 				$(MANDATORY)./gameplay/enemy/

GAMEPLAY_DIR_BONUS =			$(BONUS)./gameplay/
RENDER_DIR_BONUS =				$(BONUS)./render/
ENEMY_DIR_BONUS = 				$(BONUS)./gameplay/enemy/


# ############################################################################ #
#						# PROJECT FILES DECLARATIONS #				   #
# ############################################################################ #

#Gathers all files in $(FILES)
FILES =	$(addprefix $(CHECKS_DIR), $(CHECKS_FILES)) \
		$(addprefix $(FREES_DIR), $(FREES_FILES)) \
		$(addprefix $(PARSING_DIR), $(PARSING_FILES)) \
		$(addprefix $(MATH_DIR), $(MATH_FILES)) \
		$(addprefix $(RENDERING_DIR), $(RENDERING_FILES)) \
		$(addprefix $(DISPLAY_INTERACTION_DIR), $(DISPLAY_INTERACTION_FILES)) \

FILES_BONUS =	$(addprefix $(CHECKS_DIR_BONUS), $(CHECKS_FILES)) \
				$(addprefix $(CHECKS_DIR_BONUS), $(CHECKS_BONUS)) \
				$(addprefix $(FREES_DIR_BONUS), $(FREES_FILES)) \
				$(addprefix $(PARSING_DIR_BONUS), $(PARSING_FILES)) \
				$(addprefix $(PARSING_DIR_BONUS), $(PARSING_BONUS)) \
				$(addprefix $(MATH_DIR_BONUS), $(MATH_FILES)) \
				$(addprefix $(RENDERING_DIR_BONUS), $(RENDERING_FILES)) \
				$(addprefix $(RENDERING_DIR_BONUS), $(RENDERING_BONUS)) \
				$(addprefix $(DISPLAY_INTERACTION_DIR_BONUS), $(DISPLAY_INTERACTION_FILES)) \

INCLUDES_DEFAULT =	-I$(INCLUDES_DIR) \
					-I$(MLX_DIR)include/MLX42/
# INCLUDES_DEFAULT =	-I$(INCLUDES_DIR)miniRT.h \
# 					-I$(MLX_DIR)include/MLX42/MLX42.h

# Library files
AR =	$(LIBFT_DIR)libft.a
MLX42 =	./07_MLX42/build/libmlx42.a

# ############################################################################ #
#						# PROJECT END_FILES DECLARATIONS #			   #
# ############################################################################ #

NAME =	ft_shmup
NAME_BONUS =	ft_shmup_bonus

PROGRAM =	./$(NAME)
PROGRAM_BONUS =	./$(NAME_BONUS)

LIBS = -lncurses

OBJS = $(FILES:.c=.o)
OBJS_BONUS = $(FILES_BONUS:.c=.o)
MAIN_O = $(MAIN:.c=.o)
MAIN_BONUS_O = $(MAIN_BONUS:.c=.o)

# ############################################################################ #
#						# PROJECT MACROS #					   #
# ############################################################################ #

CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++2b -MMD -MP
RM =	rm -f

# ############################################################################ #
#						# PROJECT RULES DECLARATIONS #				   #
# ############################################################################ #

.c.o: 
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

all: $(NAME)
#z	@echo "\n\tCompilation done!\n Suggestion: try 'make run' or 'make valgrind'.\n"

$(NAME): $(LIBS) $(OBJS) $(MAIN_O)
	$(CC) $(CFLAGS) $(OBJS) $(MAIN_O) $(LIBS) $(INCLUDES_DEFAULT) -o $(NAME) $(MLX42_FLAG) $(LM)

$(LIBS):
	$(ARMAKE)
	$(MLX42MAKE)

microtest: $(LIBS) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(INCLUDES_DEFAULT) test.c -o test.exe

clean: clear
	$(ARMAKE) clean
	$(MLX42MAKE) clean
	$(RM) $(OBJS)
	$(RM) $(OBJS_BONUS)
	$(RM) $(MANDATORY)./miniRT.o
	$(RM) $(BONUS)./miniRT_bonus.o

fclean: clean
	$(RM) $(NAME)
	$(RM) $(NAME_BONUS)
	$(RM) a.out test.exe checker_test.exe parser_test.exe
	$(ARMAKE) fclean
	$(MLX42MAKE) fclean

re: clear fclean all

.PHONY: all clean fclean re

# ############################################################################ #
#						# PROJECT BONUS #					   #

bonus: $(NAME_BONUS)
	
$(NAME_BONUS): $(LIBS) $(OBJS_BONUS) $(MAIN_BONUS_O)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(MAIN_BONUS_O) $(LIBS) $(INCLUDES_DEFAULT) -o $(NAME_BONUS) $(MLX42_FLAG) $(LM)

.PHONY: bonus

# SRC_MDTRY_GAMEPLAY_ENEMY = bomber.cpp dodger.cpp shooter.cpp

# SRC_MDTRY_GAMEPLAY = gameplay.cpp \
# 				obstacle.cpp
# SRC_MDTRY_GAMEPLAY_ENEMY_PATHS = $(addprefix gameplay/enemy/, $(SRC_MDTRY_GAMEPLAY_ENEMY))

# SRC_MDTRY_RENDER = render.cpp


# SRC_MDTRY = $(SRC_MDTRY_GAMEPLAY) $(SRC_MDTRY_GAMEPLAY_ENEMY_PATHS) $(SRC_MDTRY_RENDER) main.cpp

# SRC_MDTRY_DIR = $(addprefix src/gameplay/, $(filter %.cpp,$(SRC_MDTRY_GAMEPLAY))) \
# 			$(addprefix src/gameplay/enemy/, $(SRC_MDTRY_GAMEPLAY_ENEMY)) \
# 			$(addprefix src/render/, $(SRC_MDTRY_RENDER)) \
# 			src/main.cpp


# # BONUS
# SRC_BONUS_GAMEPLAY = gameplay.cpp gameplay_player2.cpp obstacle.cpp
# SRC_BONUS_GAMEPLAY_ENEMY_PATHS = $(addprefix enemy/, $(SRC_MDTRY_GAMEPLAY_ENEMY))
# SRC_BONUS_RENDER = render.cpp
# SRC_BONUS = $(SRC_BONUS_GAMEPLAY) $(SRC_BONUS_GAMEPLAY_ENEMY_PATHS) $(SRC_BONUS_RENDER) main.cpp
# SRC_BONUS_DIR = $(addprefix src_bonus/gameplay/, $(filter %.cpp,$(SRC_BONUS_GAMEPLAY))) \
# 			$(addprefix src_bonus/gameplay/enemy/, $(SRC_MDTRY_GAMEPLAY_ENEMY)) \
# 			$(addprefix src_bonus/render/, $(SRC_BONUS_RENDER)) \
# 			src_bonus/main.cpp

# NAME     = ft_shmup
# CXX      = c++
# CXXFLAGS = -Wall -Wextra -Werror -std=c++2b -MMD -MP


# INC_DIR = include
# OBJ_DIR = obj

# SRC = $(SRC_MDTRY_DIR)

# OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
# OBJ_BONUS = $(addprefix $(OBJ_DIR)/bonus/, $(SRC_BONUS_DIR:src_bonus/%.cpp=%.o))
# DEP = $(OBJ:.o=.d)
# DEP_BONUS = $(OBJ_BONUS:.o=.d)

# all: $(NAME)

# $(NAME): $(OBJ)
# 	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# $(OBJ_DIR)/%.o: src/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# $(OBJ_DIR)/bonus/%.o: src_bonus/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# bonus: $(OBJ_BONUS)
# 	$(CXX) $(CXXFLAGS) -o $(NAME)_bonus $^ $(LIBS)

# clean:
# 	rm -rf $(OBJ_DIR)

# fclean: clean
# 	rm -rf $(NAME) $(NAME)_bonus

# re: fclean all

# .PHONY: all clean fclean re bonus

# -include $(DEP)
# -include $(DEP_BONUS)
