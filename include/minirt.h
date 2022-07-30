/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:36:11 by seseo             #+#    #+#             */
/*   Updated: 2022/07/30 21:51:26 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <float.h>
# include "minirt_parsing.h"
# include "minirt_hit.h"

# define _USE_MATH_DEFINES

# define SCRN_WIDTH			800
# define SCRN_HEIGHT		600

# define TRUE				1
# define FALSE				0

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

// # define C_RED				0xFF0000
// # define C_GREEN			0xFF00
// # define C_BLUE				0xFF

# define EPSILON			1e-6

typedef struct s_phong
{
	double	ka;
	double	kd;
	double	ks;
	double	alpha;
	t_color	ambient;
	t_color	diffuse;
	t_color	specular;
}	t_phong;

typedef struct s_obj_info
{
	int		type;
	t_vec	pos;
	t_vec	orient;
	t_color	color;
	double	radius;
	double	r_sqare;
	double	height;
	double	width;
}	t_obj_info;

typedef struct s_ambi_light_info
{
	t_color	color;
	double	bright;
}	t_ambi_light_info;

typedef struct s_light_info
{
	t_vec	pos;
	t_color	color;
	double	bright;
}	t_light_info;

typedef struct s_cam_info
{
	t_vec	pos;
	t_vec	orient;
	t_vec	orient_neg;
	double	angle;
	t_vec	screen;
	t_vec	x_vec;
	t_vec	y_vec;
	double	focal_len;
}	t_cam_info;

typedef struct s_map_info
{
	t_ambi_light_info	ambi_light;
	t_light_info		*light;
	t_cam_info			*cam;
	t_obj_info			*obj;
	int					light_cnt;
	int					cam_cnt;
	int					obj_cnt;
}	t_map_info;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

typedef struct s_vars
{
	void		*mlx;
	void		*win;
	t_map_info	*map;
	t_data		img;
}	t_vars;

// minirt_utils.c
int		get_color(int color, int target_color);
t_color	set_color(double red, double green, double blue);
t_color	apply_bright(t_color light, double bright);
t_color	add_color(t_color c1, t_color c2, t_color c3);
void	free_map_info(t_map_info *map);
void	free_map(t_map *map);

// minirt_convert_data.c
void	set_ambi_light_and_light_info(t_vars *vars, t_map *map);
void	set_cam_info(t_map_info *map, t_camera *cam);
void	set_obj_info(t_map_info *map, t_obj *obj);

void	print_map_info(t_map_info *map);

int		is_hit(t_ray ray, t_obj_info *obj, t_hit_info *info);

#endif
