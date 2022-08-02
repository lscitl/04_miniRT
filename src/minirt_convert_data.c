/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_convert_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:51:57 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 21:21:33 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_cam_axis(t_cam_info *cam);
static int	set_cylinder_and_cone_info(t_obj_info *obj_info);

void	set_ambi_light_and_light_info(t_vars *vars, t_map *map)
{
	t_light_info	*light;
	t_light			*light_parsing;
	int				i;

	vars->map->ambi_light.color.r = get_color(map->ambi_light.color, RED) / 255;
	vars->map->ambi_light.color.g = get_color(map->ambi_light.color, GREEN) / 255;
	vars->map->ambi_light.color.b = get_color(map->ambi_light.color, BLUE) / 255;
	vars->map->ambi_light.bright = map->ambi_light.bright;
	vars->map->light_cnt = lst_cnt((t_list *)map->light);
	light = ft_malloc(sizeof(t_light_info) * vars->map->light_cnt);
	light_parsing = map->light;
	i = 0;
	while (light_parsing)
	{
		light[i].bright = light_parsing->bright;
		light[i].color.r = get_color(light_parsing->color, RED) / 255 * light_parsing->bright;
		light[i].color.g = get_color(light_parsing->color, GREEN) / 255 * light_parsing->bright;
		light[i].color.b = get_color(light_parsing->color, BLUE) / 255 * light_parsing->bright;
		light[i].pos = vec_make(light_parsing->pos);
		light_parsing = light_parsing->next;
		i++;
	}
	vars->map->light = light;
}

void	set_cam_info(t_map_info *map, t_camera *cam)
{
	t_cam_info	*c;
	t_vec		s_center;

	map->cam_cnt = lst_cnt((t_list *)cam);
	map->cam = ft_malloc(sizeof(t_cam_info) * map->cam_cnt);
	c = map->cam;
	while (cam)
	{
		c->pos = vec_make(cam->pos);
		c->orient = vec_make(cam->orient);
		c->orient_neg = vec_scale(c->orient, -1);
		c->angle = cam->angle / 180 * M_PI;
		c->focal_len = SCRN_WIDTH / (2 * atan(c->angle / 2));
		set_cam_axis(c);
		s_center = vec_scale(c->orient, c->focal_len);
		s_center = vec_plus(s_center, vec_scale(c->x_vec, SCRN_WIDTH / 2 * -1));
		c->screen = \
			vec_plus(s_center, vec_scale(c->y_vec, SCRN_HEIGHT / 2 * -1));
		c++;
		cam = cam->next;
	}
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
		// printf("%d\n", obj->type);
		map->obj[i].type = obj->type;
		map->obj[i].pos = vec_make(obj->pos);
		map->obj[i].orient = vec_make(obj->orient);
		map->obj[i].color.r = get_color(obj->color, RED) / 255.0;
		map->obj[i].color.g = get_color(obj->color, GREEN) / 255.0;
		map->obj[i].color.b = get_color(obj->color, BLUE) / 255.0;
		map->obj[i].radius = obj->diameter / 2;
		map->obj[i].r_sqare = map->obj[i].radius * map->obj[i].radius;
		map->obj[i].height = obj->height;
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
		obj_info[0].rsq_div_hsq = pow(obj_info[0].radius, 2) \
										/ pow(obj_info[0].height, 2);
		obj_info[0].rsq_div_h = pow(obj_info[0].radius, 2) \
										/ obj_info[0].height;
		// obj_info[0].cos_square_theta = (obj_info->height * obj_info->height);
		// obj_info[0].cos_square_theta /= (obj_info->height * obj_info->height + obj_info->r_sqare);
		obj_info[1] = obj_info[0];
		// obj_info[1].pos = vec_plus(obj_info[0].pos, \
		// 					vec_scale(obj_info[0].orient, obj_info[0].height));
		obj_info[1].type = CIRCLE;
		return (1);
	}
	return (0);
}
