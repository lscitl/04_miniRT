/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_draw_img.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:09:43 by seseo             #+#    #+#             */
/*   Updated: 2022/08/03 23:20:35 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void		set_ray(t_cam_info *cam, t_ray *ray, int x, int y);
static void		get_hit_info(t_map_info *map, t_hit_info *info, t_ray ray);
static void		draw_horizon_line(t_vars *vars, t_hit_info *info, int x, int y);
static t_color	calculate_color(t_phong *param, t_hit_info *info);

void	open_texture_img(t_data *img)
{
	void	*mlx;
	int		x_size;
	int		y_size;

	mlx = mlx_init();
	img->img = mlx_xpm_file_to_image(mlx, "texture/texture.xpm", &x_size, &y_size);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
}

void	draw_image(t_vars *vars, t_map_info *map, t_cam_info *cam)
{
	// t_data		texture;
	t_hit_info	info;
	t_ray		ray;
	int			x;
	int			y;
	// int			tex_cord[2];

	ft_memset(&info, 0, sizeof(t_hit_info));
	// open_texture_img(&texture);
	y = 0;
	while (y < SCRN_HEIGHT)
	{
		x = 0;
		while (x < SCRN_WIDTH)
		{
			set_ray(cam, &ray, x, y);
			get_hit_info(map, &info, ray);
			// get_tex_cord(info, )
			// info.color = set_color2(get_mlx_pixel_color(texture, x, y));
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
	int		obj_index;

	info->distance = DBL_MAX;
	obj_index = 0;
	while (obj_index < map->obj_cnt)
		is_hit(ray, &map->obj[obj_index++], info);
}

static void	draw_horizon_line(t_vars *vars, t_hit_info *info, int x, int y)
{
	t_color	color;
	t_phong	param;
	int		y_mlx;

	y_mlx = SCRN_HEIGHT - y - 1;
	if (info->distance < DBL_MAX)
	{
		param = phong_reflection(vars->map, info, vars->map->cam->orient_neg);
		color = calculate_color(&param, info);
		my_mlx_pixel_put(&vars->img, x, y_mlx, convert_color(color));
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
