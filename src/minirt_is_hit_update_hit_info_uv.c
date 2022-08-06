/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info_uv.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 23:52:03 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 15:16:11 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void	find_uv_unit_vec(t_vec orient, t_vec *u_unit, t_vec *v_unit);

// U, V in range [0, 1]
// plane -> UV coordinate
// sphere -> UV mapping
// cylinder -> theta, h
// cone -> r (distance from top_point), theta on the
void	get_uv_mapping(t_obj_info *obj, t_hit_info *info, t_uv_map *uv_map)
{
	t_vec	tmp;
	// t_vec	p;
	t_vec	h;
	t_vec	d;
	double	costh;

	if (obj->type != SPHERE)
		find_uv_unit_vec(obj->orient, &info->uv_map.u_vec, &info->uv_map.v_vec);
	else
	{
		info->uv_map.u_vec.x = 1;
		info->uv_map.u_vec.y = 0;
		info->uv_map.u_vec.z = 0;
		info->uv_map.v_vec.x = 0;
		info->uv_map.v_vec.y = 1;
		info->uv_map.v_vec.z = 0;
	}
	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		uv_map->u = \
			vec_dotprod(vec_minus(info->hit_point, obj->pos), uv_map->u_vec);
		uv_map->v = \
			vec_dotprod(vec_minus(info->hit_point, obj->pos), uv_map->v_vec);
	}
	else if (obj->type == SPHERE)
	{
		d = vec_normalize(vec_minus(info->hit_point, obj->pos));
		uv_map->u = (M_PI / 2.0 + (atan2(d.z, -d.x))) * obj->radius;
		uv_map->v = acos(-d.y) * obj->radius;
		// uv_map->u = 0.5 + atan2(d.x, d.z) / (2.0 * M_PI);
		// uv_map->v = 0.5 + asin(d.y) / M_PI;
	}
	else
	{
		tmp = vec_minus(info->hit_point, obj->pos);
		h = vec_scale(obj->orient, vec_dotprod(tmp, obj->orient));
		costh = vec_dotprod(vec_normalize(vec_minus(tmp, h)), uv_map->u_vec);
		uv_map->u = acos(costh) * obj->radius;
		uv_map->v = vec_dotprod(vec_minus(info->hit_point, obj->pos), obj->orient);
	}
}

static void	find_uv_unit_vec(t_vec orient, t_vec *u_unit, t_vec *v_unit)
{
	const t_vec	x_vec = {1, 0, 0};
	const t_vec	y_vec = {0, 1, 0};

	if (is_zero(fabs(orient.y) - 1) == FALSE)
	{
		*u_unit = vec_normalize(vec_crossprod(y_vec, orient));
		*v_unit = vec_normalize(vec_crossprod(orient, *u_unit));
	}
	else
	{
		*v_unit = x_vec;
		*u_unit = vec_crossprod(orient, *v_unit);
	}
}
