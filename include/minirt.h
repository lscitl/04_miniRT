/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:36:11 by seseo             #+#    #+#             */
/*   Updated: 2022/07/25 16:02:23 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <mlx.h>
# include "libft.h"

# define _USE_MATH_DEFINES

# define TRUE	1
# define FALSE	0

# define KEY_A				0
# define KEY_S				1
# define KEY_Z				6
# define KEY_X				7
# define KEY_C				8
# define KEY_Q				12
# define KEY_W				13
# define KEY_R				15
# define KEY_O				31
# define KEY_I				34
# define KEY_P				35

# define KEY_LEFT			123
# define KEY_RIGHT			124
# define KEY_DOWN			125
# define KEY_UP				126

# define KEY_ESC			53
# define KEY_EVENT_PRESS	2
# define KEY_EVENT_EXIT		17

# define C_RED				0xFF0000
# define C_GREEN			0xFF00
# define C_BLUE				0xFF

enum e_color
{
	BLUE,
	GREEN,
	RED
};

enum e_obj_type
{
	NONE,
	PLANE,
	SPHERE,
	CYLINDER,
	CONE
};

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

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

typedef struct s_vars
{
	void	*mlx;
	void	*win;
	t_map	*map;
	t_data	img;
}	t_vars;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_ray
{
	t_vec	orig;
	t_vec	direction;
}	t_ray;

typedef struct s_hit_info
{
	t_vec	hit_point;
	t_color	color;
	t_vec	norm_vec; // vec_dotprod(normal_vector, ray.direction);
	double	t; // (orig + t * dir) hit object
}	t_hit_info;

t_map	*map_parsing(char *file_path);
int		set_map_data(t_map *map, char **map_args);
int		get_color(int color, int target_color);

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
void	ft_objadd_back(t_obj **lst, t_obj *new);
void	ft_camadd_back(t_camera **lst, t_camera *new);
void	ft_lightadd_back(t_light **lst, t_light *new);

// minirt_utils_1.c
void	replace_all_white_space_to_sp(char *str);
int		args_len_check(char **args, int len);
int		is_all_digit(char *str);
int		orient_check(double orient[3]);

// debugging
void	print_strs(char **strs);
void	print_map(t_map *map);

#endif
