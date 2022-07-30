# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/30 15:35:48 by seseo             #+#    #+#              #
#    Updated: 2022/07/30 23:28:03 by seseo            ###   ########.fr        #
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
					is_hit.c \
					print_map_for_debugging.c

SRCS			:=	$(addprefix $(SRCS_DIR)/, $(SRC))
OBJS			:=	$(SRC:%.c=$(OBJS_DIR)/%.o)

NAME			:=	miniRT
LFT_PATH		:=	libft
L_FT			:=	$(LFT_PATH)/libft.a
MLX_PATH		:=	mlx
L_MLX			:=	$(MLX_PATH)/libmlx.dylib
CC				:=	cc
RM				:=	rm -rf
CFLAGS			:=	-Wall -Wextra -Werror -g -fsanitize=address
INC				:=	-I$(LFT_PATH)/include -Iinclude -I$(MLX_PATH)
LIB_PATH		:=	-L$(MLX_PATH) -L$(LFT_PATH)
LIB				:=	-lft -lmlx
FRAMEWORK		:=	-framework OpenGL -framework AppKit

$(OBJS_DIR)/%.o	:	$(SRCS_DIR)/%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME)			:	$(OBJS) $(L_FT) $(L_MLX)
					@cp $(L_MLX) ./
					$(CC) $(CFLAGS) $(OBJS) $(INC) $(LIB_PATH) $(LIB) $(FRAMEWORK) -o $(NAME)

$(L_FT)			:
					@$(MAKE) bonus -C $(LFT_PATH)

$(L_MLX)		:
					@$(MAKE) -C $(MLX_PATH)

all				:	$(NAME)

clean			:
					$(RM) $(OBJS)
					@$(MAKE) -C $(MLX_PATH) clean
					@$(MAKE) -C $(LFT_PATH) clean

fclean			:	clean
					@$(MAKE) -C $(LFT_PATH) fclean
					$(RM) libmlx.dylib
					$(RM) $(NAME)

re				:	fclean
					@$(MAKE) all

bonus			:	all

.PHONY			:	all clean fclean re bonus
