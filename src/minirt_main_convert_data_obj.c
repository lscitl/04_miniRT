/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_convert_data_obj.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 14:51:57 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 17:16:40 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	set_obj_info_general_var(t_obj_info *obj_info, t_obj *obj);
static int	set_cylinder_and_cone_info(t_obj_info *obj_info);
static void	find_uv_unit_vec(t_obj_info *obj);

void	set_obj_info(t_map_info *map, t_obj *obj)
{
	int		i;

	map->obj_cnt = lst_cnt_obj(obj);
	map->obj = ft_malloc(sizeof(t_obj_info) * map->obj_cnt);
	i = 0;
	while (obj)
	{
		set_obj_info_general_var(&map->obj[i], obj);
		if (obj->tx)
			open_texture_img(&map->obj[i].tx, obj->tx);
		if (obj->bm)
			open_texture_img(&map->obj[i].bm, obj->bm);
		i += set_cylinder_and_cone_info(&map->obj[i]);
		i++;
		obj = obj->next;
	}
}

static void	set_obj_info_general_var(t_obj_info *obj_info, t_obj *obj)
{
	obj_info->type = obj->type;
	obj_info->pos = vec_make(obj->pos);
	obj_info->orient = vec_make(obj->orient);
	obj_info->color = set_color_from_int(obj->color);
	obj_info->radius = obj->diameter / 2.0;
	obj_info->r_sqare = obj_info->radius * obj_info->radius;
	obj_info->height = obj->height;
	obj_info->surface = obj->surface;
	obj_info->map_scale = obj->map_scale;
	obj_info->kd = obj->kd;
	obj_info->ks = obj->ks;
	obj_info->alpha = obj->alpha;
	find_uv_unit_vec(obj_info);
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

static void	find_uv_unit_vec(t_obj_info *obj)
{
	const t_vec	x_vec = {1, 0, 0};
	const t_vec	y_vec = {0, 1, 0};

	if (obj->type == SPHERE)
	{
		ft_memset(&(obj->uv_map), 0, sizeof(obj->uv_map));
		obj->uv_map.u_vec.x = 1;
		obj->uv_map.v_vec.y = 1;
		return ;
	}
	if (is_zero(fabs(obj->orient.y) - 1) == FALSE)
	{
		obj->uv_map.u_vec = vec_normalize(vec_crossprod(y_vec, obj->orient));
		obj->uv_map.v_vec = vec_normalize(vec_crossprod(obj->orient, \
					obj->uv_map.u_vec));
	}
	else
	{
		obj->uv_map.v_vec = x_vec;
		obj->uv_map.u_vec = vec_crossprod(obj->orient, obj->uv_map.v_vec);
	}
}
