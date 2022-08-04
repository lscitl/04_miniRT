/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_parsing.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 20:24:36 by seseo             #+#    #+#             */
/*   Updated: 2022/08/04 15:05:11 by chanhpar         ###   ########.fr       */
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
	SQUARE,
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
t_map	*map_parsing(char *file_path);

// minirt_map_parsing_set_map_data.c
int		set_map_data(t_map *map, char **map_args);

// minirt_map_parsing_set_light_and_cam.c
int		set_ambient_light(t_map *map, char **args);
int		set_light(t_map *map, char **args);
int		set_camera(t_map *map, char **args);

// minirt_map_parsing_set_obj_*.c
int		set_cylinder_and_cone(t_map *map, t_obj *obj, char **args);
int		set_plane(t_map *map, t_obj *plane, char **args);
int		set_sphere(t_map *map, t_obj *sphere, char **args);

// minirt_map_parsing_set_obj.c
int		set_obj(t_map *map, char **args);

// minirt_parsing_utils_lst.c
int		lst_cnt_obj(t_obj *lst);
int		lst_cnt(t_list *lst);
void	ft_lst_node_clear(t_list *lst);

// minirt_utils_1.c
void	replace_all_white_space_to_sp(char *str);
int		args_len_check(char **args, int len);
int		is_all_digit(char *str);
int		orient_check(double orient[3]);

// debugging
void	print_strs(char **strs);
void	print_map(t_map *map);

#endif
