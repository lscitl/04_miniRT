/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_draw_img.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:09:43 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 16:36:59 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void		set_ray(t_cam_info *cam, t_ray *ray, int x, int y);
static void		get_hit_info(t_map_info *map, t_hit_info *info);
static void		put_pixel_in_img(t_vars *vars, t_hit_info *info, int x, int y);
static t_color	calculate_color(t_phong *param, t_hit_info *info);
static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);

void	draw_image(t_vars *vars, t_map_info *map, t_cam_info *cam)
{
	t_hit_info	info;
	int			x;
	int			y;

	ft_memset(&info, 0, sizeof(t_hit_info));
	y = 0;
	while (y < SCRN_HEIGHT)
	{
		x = 0;
		while (x < SCRN_WIDTH)
		{
			set_ray(cam, &info.ray, x, y);
			get_hit_info(map, &info);
			put_pixel_in_img(vars, &info, x, y);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img, 0, 0);
}

static void	set_ray(t_cam_info *cam, t_ray *ray, int x, int y)
{
	t_vec	screen_point;
	t_vec	x_vec;
	t_vec	y_vec;

	ray->orig = cam->pos;
	x_vec = vec_scale(cam->x_vec, x);
	y_vec = vec_scale(cam->y_vec, y);
	screen_point = vec_plus(vec_plus(cam->screen, x_vec), y_vec);
	ray->direction = vec_normalize(screen_point);
}

static void	get_hit_info(t_map_info *map, t_hit_info *info)
{
	int		obj_index;

	info->distance = DBL_MAX;
	// info->shadow_flag = FALSE;
	info->obj = NULL;
	obj_index = 0;
	while (obj_index < map->obj_cnt)
		is_hit(&map->obj[obj_index++], info);
	if (info->obj)
	{
		info->phong.kd = info->obj->kd;
		info->phong.ks = info->obj->ks;
		info->phong.alpha = info->obj->alpha;
		info->norm_vec = get_normal_vector(info->ray.direction, \
											info->hit_point, info->obj);
		if (info->obj->surface)
			info->color = get_point_color(info->obj, info);
		else
			info->color = info->obj->color;
	}
}

static void	put_pixel_in_img(t_vars *vars, t_hit_info *info, int x, int y)
{
	t_color	color;
	int		y_mlx;

	y_mlx = SCRN_HEIGHT - y - 1;
	if (info->distance < DBL_MAX)
	{
		phong_reflection(vars->map, info, vars->map->cam->orient_neg);
		color = calculate_color(&info->phong, info);
		my_mlx_pixel_put(&vars->img, x, y_mlx, convert_color_to_int(color));
	}
	else
		my_mlx_pixel_put(&vars->img, x, y_mlx, 0);
}

static t_color	calculate_color(t_phong *param, t_hit_info *info)
{
	t_color	color;

	color.r = param->point_color.r * info->color.r;
	color.g = param->point_color.g * info->color.g;
	color.b = param->point_color.b * info->color.b;
	color = add_color(color, param->specular);
	if (color.r > 1)
		color.r = 1;
	if (color.g > 1)
		color.g = 1;
	if (color.b > 1)
		color.b = 1;
	color.r = color.r * 255;
	color.g = color.g * 255;
	color.b = color.b * 255;
	return (color);
}

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj)
{
	t_vec	norm;
	t_vec	cone_top;
	t_vec	diff;

	diff = vec_minus(point, obj->pos);
	if (obj->type == SPHERE)
		norm = vec_normalize(diff);
	else if (obj->type == CYLINDER)
	{
		norm = vec_normalize(vec_crossprod(obj->orient, \
											vec_crossprod(diff, obj->orient)));
	}
	else if (obj->type == CONE)
	{
		cone_top = vec_plus(obj->pos, vec_scale(obj->orient, obj->height));
		norm = vec_normalize(vec_crossprod(vec_minus(cone_top, point), \
											vec_crossprod(diff, obj->orient)));
	}
	else
		norm = obj->orient;
	if (vec_dotprod(norm, dir) > 0)
		return (vec_scale(norm, (double)-1));
	return (norm);
}
