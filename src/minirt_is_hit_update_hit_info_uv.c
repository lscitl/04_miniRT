/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info_uv.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 23:52:03 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 13:05:57 by chanhpar         ###   ########.fr       */
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
	t_vec	p;
	t_vec	h;
	t_vec	d;

	find_uv_unit_vec(obj->orient, &info->uv_map.u_vec, &info->uv_map.v_vec);
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
		// d = vec_normalize(vec_minus(obj->pos, info->hit_point));
		uv_map->u = (M_PI + atan2(d.z, -d.x)) / (M_PI);
		uv_map->v = acos(-d.y) / M_PI;
		// uv_map->u = 0.5 + atan2(d.x, d.z) / (2.0 * M_PI);
		// uv_map->v = 0.5 + asin(d.y) / M_PI;
	}
	else
	{
		h = vec_scale(obj->orient, -vec_dotprod(info->hit_point, obj->orient));
		p = vec_minus(vec_plus(info->hit_point, h), obj->pos);
		// uv_map->v = atan2(-vec_dotprod(p, info->uv_map.v_vec), vec_dotprod(p, info->uv_map.u_vec)) / (M_PI);
		uv_map->u = acos(vec_dotprod(vec_normalize(vec_minus(vec_minus(info->hit_point, obj->pos), vec_scale(obj->orient, vec_dotprod((vec_minus(info->hit_point, obj->pos)), obj->orient)))), uv_map->u_vec)) / M_PI;
		uv_map->v = vec_dotprod(obj->orient, vec_minus(info->hit_point, obj->pos)) / obj->height;
		if (obj->type == CONE)
			uv_map->v = 1.0 - uv_map->v;
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
