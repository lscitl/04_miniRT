/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:03:47 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/05 23:56:44 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);
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
	if (info->shadow_flag == TRUE)
		return (0);
	info->phong.kd = obj->kd;
	info->phong.ks = obj->ks;
	info->phong.alpha = obj->alpha;
	info->norm_vec = get_normal_vector(info->ray.direction, hit_point, obj);
	if (obj->surface)
		info->color = get_point_color(obj, info);
	else
		info->color = obj->color;
	return (0);
}

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj)
{
	t_vec	norm;
	t_vec	cone_top;
	t_vec	diff;

	diff = vec_minus(point, obj->pos);
	if (obj->type == SPHERE)
		norm = vec_normalize(diff);
	else if (obj->type == CYLINDER)
	{
		norm = vec_normalize(vec_crossprod(obj->orient, \
											vec_crossprod(diff, obj->orient)));
	}
	else if (obj->type == CONE)
	{
		cone_top = vec_plus(obj->pos, vec_scale(obj->orient, obj->height));
		norm = vec_normalize(vec_crossprod(vec_minus(cone_top, point), \
											vec_crossprod(diff, obj->orient)));
	}
	else
		norm = obj->orient;
	if (vec_dotprod(norm, dir) > 0)
		return (vec_scale(norm, (double)-1));
	return (norm);
}

static int	is_within_obj(t_vec intersect, t_obj_info *obj)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, obj->pos), obj->orient);
	return (projection >= -EPSILON && projection <= obj->height);
}
