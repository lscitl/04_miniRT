# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/30 15:35:48 by seseo             #+#    #+#              #
#    Updated: 2022/07/27 16:18:30 by seseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR 		:=	src
OBJS_DIR		:=	obj

SRC				:=	minirt_main.c \
					minirt_calculate_color.c \
					minirt_convert_data.c \
					minirt_map_parsing_main.c \
					minirt_map_parsing_set_light_and_cam.c \
					minirt_map_parsing_set_map_data.c \
					minirt_map_parsing_set_obj_cylinder_and_cone.c \
					minirt_map_parsing_set_obj_plane.c \
					minirt_map_parsing_set_obj_sphere.c \
					minirt_map_parsing_set_obj.c \
					minirt_map_parsing_utils_lst.c \
					minirt_map_parsing_utils.c \
					minirt_math_vec_utils_1.c \
					minirt_math_vec_utils_2.c \
					minirt_math_vec_utils_3.c \
					minirt_util_1.c \
					print_map_for_debugging.c

SRCS			:=	$(addprefix $(SRCS_DIR)/, $(SRC))
OBJS			:=	$(SRC:%.c=$(OBJS_DIR)/%.o)

NAME			:=	miniRT
L_PATH			:=	libft
L_FT			:=	$(L_PATH)/libft.a
M_PATH			:=	mlx
L_MLX			:=	$(M_PATH)/libmlx.dylib
CC				:=	cc
RM				:=	rm -rf
CFLAGS			:=	-Wall -Wextra -Werror -g# -fsanitize=address
INC				:=	-I$(L_PATH)/include -Iinclude -I$(M_PATH)
LIB_PATH		:=	-L$(M_PATH) -L$(L_PATH)
LIB				:=	-lft -lmlx -lm
FRAMEWORK		:=	-framework OpenGL -framework AppKit

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.c
					@mkdir -p $(@D)
					$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME)			:	$(OBJS) $(L_FT) $(L_MLX)
					@cp $(L_MLX) ./
					$(CC) $(OBJS) $(INC) $(LIB_PATH) $(LIB) $(FRAMEWORK) -o $(NAME)

$(L_FT)			:
					@$(MAKE) bonus -C $(L_PATH)

$(L_MLX)		:
					@$(MAKE) -C $(M_PATH)

all				:	$(NAME)

clean			:
					$(RM) $(OBJS)
					@$(MAKE) -C $(M_PATH) clean
					@$(MAKE) -C $(L_PATH) clean

fclean			:	clean
					@$(MAKE) -C $(L_PATH) fclean
					$(RM) libmlx.dylib
					$(RM) $(NAME)

re				:	fclean
					@$(MAKE) all

bonus			:	all

.PHONY			:	all clean fclean re bonus
