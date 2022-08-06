/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info_uv.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 23:52:03 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 17:15:52 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void	get_uv_sphere(t_obj_info *obj, t_hit_info *info, \
		t_uv_map *uv_map);
static void	get_uv_plane_circle(t_obj_info *obj, t_hit_info *info, \
		t_uv_map *uv_map);
static void	get_uv_cylinder_cone(t_obj_info *obj, t_hit_info *info, \
		t_uv_map *uv_map);

// U, V in range [0, 1]
// plane -> UV coordinate
// sphere -> UV mapping
// cylinder -> theta, h
// cone -> r (distance from top_point), theta on the
void	get_uv_mapping(t_obj_info *obj, t_hit_info *info, t_uv_map *uv_map)
{
	*uv_map = obj->uv_map;
	if (obj->type == SPHERE)
	{
		get_uv_sphere(obj, info, uv_map);
	}
	else if (obj->type == PLANE || obj->type == CIRCLE)
	{
		get_uv_plane_circle(obj, info, uv_map);
	}
	else
	{
		get_uv_cylinder_cone(obj, info, uv_map);
	}
}

static void	get_uv_sphere(t_obj_info *obj, t_hit_info *info, t_uv_map *uv_map)
{
	t_vec	tmp;
	t_vec	d;

	tmp = vec_minus(info->hit_point, obj->pos);
	d = vec_normalize(tmp);
	uv_map->u = (M_PI / 2.0 + (atan2(d.z, -d.x))) * obj->radius;
	uv_map->v = acos(-d.y) * obj->radius;
}

static void	get_uv_plane_circle(t_obj_info *obj, t_hit_info *info, \
		t_uv_map *uv_map)
{
	t_vec	tmp;

	tmp = vec_minus(info->hit_point, obj->pos);
	uv_map->u = vec_dotprod(tmp, uv_map->u_vec);
	uv_map->v = vec_dotprod(tmp, uv_map->v_vec);
}

static void	get_uv_cylinder_cone(t_obj_info *obj, t_hit_info *info, \
		t_uv_map *uv_map)
{
	t_vec	h;
	double	costh;
	t_vec	tmp;

	tmp = vec_minus(info->hit_point, obj->pos);
	h = vec_scale(obj->orient, vec_dotprod(tmp, obj->orient));
	costh = vec_dotprod(vec_normalize(vec_minus(tmp, h)), uv_map->u_vec);
	uv_map->u = acos(costh) * obj->radius;
	uv_map->v = vec_dotprod(vec_minus(info->hit_point, obj->pos), \
			obj->orient);
}
