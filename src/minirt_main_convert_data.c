/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_convert_data.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:51:57 by seseo             #+#    #+#             */
/*   Updated: 2022/08/05 13:57:42 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_cam_axis(t_cam_info *cam);
static int	set_cylinder_and_cone_info(t_obj_info *obj_info);

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

void	set_obj_info(t_map_info *map, t_obj *obj)
{
	int		i;

	map->obj_cnt = lst_cnt_obj(obj);
	map->obj = ft_malloc(sizeof(t_obj_info) * map->obj_cnt);
	i = 0;
	while (obj)
	{
		map->obj[i].type = obj->type;
		map->obj[i].pos = vec_make(obj->pos);
		map->obj[i].orient = vec_make(obj->orient);
		map->obj[i].color = set_color_from_int(obj->color);
		map->obj[i].radius = obj->diameter / 2.0;
		map->obj[i].r_sqare = map->obj[i].radius * map->obj[i].radius;
		map->obj[i].height = obj->height;
		map->obj[i].surface = obj->surface;
		map->obj[i].map_scale = obj->map_scale;
		map->obj[i].kd = 1;//obj->kd;
		map->obj[i].ks = 0.5;//obj->ks;
		map->obj[i].alpha = 64;//obj->alpha;
		if (obj->tx)
			open_texture_img(&map->obj[i].tx, obj->tx);
		if (obj->bm)
			open_texture_img(&map->obj[i].bm, obj->bm);
		i += set_cylinder_and_cone_info(&map->obj[i]);
		i++;
		obj = obj->next;
	}
}

static int	set_cylinder_and_cone_info(t_obj_info *obj_info)
{
	if (obj_info->type == CYLINDER)
	{
		obj_info[1] = obj_info[0];
		obj_info[2] = obj_info[0];
		obj_info[1].type = CIRCLE;
		obj_info[1].orient = vec_scale(obj_info[1].orient, -1);
		obj_info[2].type = CIRCLE;
		obj_info[2].pos = vec_plus(obj_info[0].pos, \
							vec_scale(obj_info[0].orient, obj_info[0].height));
		return (2);
	}
	else if (obj_info->type == CONE)
	{
		obj_info[0].rsq_div_h = obj_info[0].r_sqare / obj_info[0].height;
		obj_info[0].rsq_div_hsq = obj_info[0].rsq_div_h / obj_info[0].height;
		obj_info[1] = obj_info[0];
		obj_info[1].type = CIRCLE;
		return (1);
	}
	return (0);
}
