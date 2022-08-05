/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_convert_data.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:51:57 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:32:07 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_cam_axis(t_cam_info *cam);

void	set_ambi_light_and_light_info(t_map_info *map_info, t_map *map, \
													t_ambi_light ambi_light)
{
	t_light_info		*light_info;
	t_light				*light;
	int					i;

	map_info->ambi_light.color = set_color_from_int(ambi_light.color);
	map_info->ambi_light.bright = ambi_light.bright;
	map_info->light_cnt = lst_cnt((t_list *)map->light);
	light_info = ft_malloc(sizeof(t_light_info) * map_info->light_cnt);
	light = map->light;
	i = 0;
	while (light)
	{
		light_info[i].bright = light->bright;
		light_info[i].color = set_color_from_int(light->color);
		light_info[i].pos = vec_make(light->pos);
		light = light->next;
		i++;
	}
	map_info->light = light_info;
}

void	set_cam_info(t_map_info *map, t_camera *cam)
{
	t_cam_info	*c;
	t_vec		center;
	int			i;

	map->cam_cnt = lst_cnt((t_list *)cam);
	c = ft_malloc(sizeof(t_cam_info) * map->cam_cnt);
	i = 0;
	while (cam)
	{
		c[i].pos = vec_make(cam->pos);
		c[i].orient = vec_make(cam->orient);
		c[i].orient_neg = vec_scale(c[i].orient, -1);
		c[i].angle = cam->angle / 180 * M_PI;
		c[i].focal_len = SCRN_WIDTH / (2 * atan(c[i].angle / 2));
		set_cam_axis(&c[i]);
		center = vec_scale(c[i].orient, c[i].focal_len);
		center = vec_plus(center, vec_scale(c[i].x_vec, SCRN_WIDTH / 2 * -1));
		c[i].screen = \
				vec_plus(center, vec_scale(c[i].y_vec, SCRN_HEIGHT / 2 * -1));
		i++;
		cam = cam->next;
	}
	map->cam = c;
}

static void	set_cam_axis(t_cam_info *cam)
{
	static double	x_axis[3] = {1, 0, 0};
	static double	y_axis[3] = {0, 1, 0};
	const t_vec		x_vec = vec_make(x_axis);
	const t_vec		y_vec = vec_make(y_axis);

	if (is_zero(fabs(cam->orient.y) - 1) == FALSE)
	{
		cam->x_vec = \
			vec_normalize(vec_crossprod(y_vec, cam->orient_neg));
		cam->y_vec = \
			vec_normalize(vec_crossprod(cam->orient_neg, cam->x_vec));
	}
	else
	{
		cam->y_vec = vec_normalize(vec_crossprod(x_vec, cam->orient));
		cam->x_vec = vec_normalize(vec_crossprod(cam->orient_neg, cam->y_vec));
	}
}
