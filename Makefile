# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: seseo <seseo@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/30 15:35:48 by seseo             #+#    #+#              #
#    Updated: 2022/08/06 01:35:57 by seseo            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS_DIR 		:=	src
OBJS_DIR		:=	obj

SRC				:=	minirt_main.c \
					minirt_is_hit_calc_coeff.c \
					minirt_is_hit_main.c \
					minirt_is_hit_phong_and_shadow.c \
					minirt_is_hit_update_hit_info_get_color.c \
					minirt_is_hit_update_hit_info_uv.c \
					minirt_is_hit_update_hit_info.c \
					minirt_main_convert_data_obj.c \
					minirt_main_convert_data.c \
					minirt_main_draw_img.c \
					minirt_main_initialize.c \
					minirt_main_mlx_hook.c \
					minirt_map_parsing_main.c \
					minirt_map_parsing_set_light_and_cam.c \
					minirt_map_parsing_set_map_data.c \
					minirt_map_parsing_set_obj.c \
					minirt_map_parsing_utils_lst.c \
					minirt_map_parsing_utils_obj_1.c \
					minirt_map_parsing_utils_obj_2.c \
					minirt_map_parsing_utils.c \
					minirt_utils_color_1.c \
					minirt_utils_color_2.c \
					minirt_utils_math_1.c \
					minirt_utils_math_2.c \
					minirt_utils_math_3.c \
					minirt_utils.c \
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
CFLAGS			:=	-Wall -Wextra -Werror -g #-fsanitize=address
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

fclean			:	clean
					@$(MAKE) -C $(LFT_PATH) fclean
					@$(MAKE) -C $(MLX_PATH) clean
					$(RM) libmlx.dylib
					$(RM) $(NAME)

re				:	fclean
					@$(MAKE) all

bonus			:	all

.PHONY			:	all clean fclean re bonus
