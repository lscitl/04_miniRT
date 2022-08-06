/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:03:47 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/06 16:15:34 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

// static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);
static int		is_within_obj(t_vec intersect, t_obj_info *obj);

int	update_hit_info(t_hit_info *info, t_obj_info *obj, double root)
{
	t_vec	hit_point;

	hit_point = vec_ray_at_distance(info->ray, root);
	if (obj->type == CYLINDER || obj->type == CONE)
	{
		if (!is_within_obj(hit_point, obj))
			return (-1);
	}
	info->hit_point = hit_point;
	info->distance = root;
	info->obj = obj;
	// if (info->shadow_flag == TRUE)
	// 	return (0);
	// info->phong.kd = obj->kd;
	// info->phong.ks = obj->ks;
	// info->phong.alpha = obj->alpha;
	// info->norm_vec = get_normal_vector(info->ray.direction, hit_point, obj);
	// if (obj->surface)
	// 	info->color = get_point_color(obj, info);
	// else
	// 	info->color = obj->color;
	return (0);
}


static int	is_within_obj(t_vec intersect, t_obj_info *obj)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, obj->pos), obj->orient);
	return (projection >= -EPSILON && projection <= obj->height);
}
