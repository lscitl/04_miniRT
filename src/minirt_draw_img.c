/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_draw_img.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:09:43 by seseo             #+#    #+#             */
/*   Updated: 2022/08/03 01:13:42 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void		set_ray(t_cam_info *cam, t_ray *ray, int x, int y);
static void		get_hit_info(t_map_info *map, t_hit_info *info, t_ray ray);
static void		draw_horizon_line(t_vars *vars, t_hit_info *info, int x, int y);
static t_color	adjust_color(t_hit_info *info, t_color color);

void	draw_image(t_vars *vars, t_map_info *map, t_cam_info *cam)
{
	t_hit_info	info;
	t_ray		ray;
	int			x;
	int			y;

	ft_memset(&info, 0, sizeof(t_hit_info));
	y = 0;
	while (y < SCRN_HEIGHT)
	{
		x = 0;
		while (x < SCRN_WIDTH)
		{
			set_ray(cam, &ray, x, y);
			get_hit_info(map, &info, ray);
			draw_horizon_line(vars, &info, x, y);
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

static void	get_hit_info(t_map_info *map, t_hit_info *info, t_ray ray)
{
	int	obj_index;

	info->distance = DBL_MAX;
	obj_index = 0;
	while (obj_index < map->obj_cnt)
		is_hit(ray, &map->obj[obj_index++], info);
}

static void	draw_horizon_line(t_vars *vars, t_hit_info *info, int x, int y)
{
	t_color	color;
	int		y_mlx;

	y_mlx = SCRN_HEIGHT - y - 1;
	if (info->distance < DBL_MAX)
	{
		color = phong_reflection(vars->map, info, vars->map->cam->orient_neg);
		color = adjust_color(info, color);
		my_mlx_pixel_put(&vars->img, x, y_mlx, convert_color(color));
	}
	else
		my_mlx_pixel_put(&vars->img, x, y_mlx, 0);
}

static t_color	adjust_color(t_hit_info *info, t_color color)
{
	if (color.r > 1)
		color.r = 1;
	if (color.g > 1)
		color.g = 1;
	if (color.b > 1)
		color.b = 1;
	// if (color.r == 1 && color.g == 1 && color.b == 1)
	// {
	// 	color.r = 255;
	// 	color.g = 255;
	// 	color.b = 255;
	// 	return (color);
	// }
	if (color.r > .93 && color.g > .93 && color.b > .93)
	{
		color.r *= 255;
		color.g *= 255;
		color.b *= 255;
		return (color);
	}
	color.r = color.r * info->color.r * 255;
	color.g = color.g * info->color.g * 255;
	color.b = color.b * info->color.b * 255;
	return (color);
}