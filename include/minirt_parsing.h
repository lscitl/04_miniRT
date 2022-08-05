/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_parsing.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 20:24:36 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:25:05 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_PARSING_H
# define MINIRT_PARSING_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

enum e_color
{
	BLUE,
	GREEN,
	RED
};

enum e_obj_type
{
	SPHERE,
	CYLINDER,
	CONE,
	PLANE,
	CIRCLE,
	NONE
};

enum e_lst_type
{
	LIGHT,
	CAMERA,
	OBJ
};

typedef struct s_obj
{
	struct s_obj	*next;
	int				type;
	double			pos[3];
	double			orient[3];
	int				color;
	double			diameter;
	double			height;
	int				surface;
	double			map_scale;
	char			*bm;
	char			*tx;
	double			kd;
	double			ks;
	double			alpha;
}	t_obj;

typedef struct s_ambi_light
{
	double	bright;
	int		color;
}	t_ambi_light;

typedef struct s_camera
{
	struct s_camera	*next;
	double			pos[3];
	double			orient[3];
	double			angle;
}	t_camera;

typedef struct s_light
{
	struct s_light	*next;
	double			pos[3];
	int				color;
	double			bright;
}	t_light;

typedef struct s_map
{
	t_ambi_light	ambi_light;
	t_camera		*camera;
	t_light			*light;
	t_obj			*obj;
}	t_map;

// minirt_map_parsing_main.c
void	map_parsing(char *file_path, t_map *map);

// minirt_map_parsing_set_map_data.c
int		set_map_data(t_map *map, char **map_args);

// minirt_map_parsing_set_light_and_cam.c
int		set_ambient_light(t_map *map, char **args);
int		set_light(t_map *map, char **args);
int		set_camera(t_map *map, char **args);

// minirt_map_parsing_set_obj.c
int		set_obj(t_map *map, char **args);

// minirt_parsing_utils_lst.c
int		lst_cnt_obj(t_obj *lst);
int		lst_cnt(t_list *lst);
void	ft_lst_node_clear(t_list *lst);
void	ft_obj_node_clear(t_obj *obj);

// minirt_utils_1.c
void	replace_all_white_space_to_sp(char *str);
int		get_args_len(char **args);
int		is_all_digit(char *str);
int		orient_check(double orient[3]);

// minirt_map_parsing_utils_obj_1.c
int		set_obj_pos(t_obj *obj, char **args);
int		set_obj_orient(t_obj *obj, char **args);
int		set_obj_default_surface_color(t_obj *obj, char **args);
int		set_obj_surface_and_color(t_obj *obj, char **args);

// minirt_map_parsing_utils_obj_2.c
int		set_cb(t_obj *obj, char **args);
int		set_bm(t_obj *obj, char **args);
int		set_tx(t_obj *obj, char **args);
int		set_phong(t_obj *obj, char **args);
int		set_phong_default(t_obj *obj);

// debugging
void	print_strs(char **strs);
void	print_map(t_map *map);

#endif
