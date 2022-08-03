/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:03:47 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/03 23:45:54 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);
static int		is_within_obj(t_vec intersect, t_obj_info *obj);
static t_color	get_point_color(t_obj_info *obj, t_vec hit_point);
static void		get_uv_mapping(t_obj_info *obj, t_vec hit_point, double uv[2]);

int	update_hit_info(t_ray ray, t_hit_info *info, t_obj_info *obj, double root)
{
	t_vec	hit_point;

	hit_point = vec_ray_at_distance(ray, root);
	if (obj->type == CYLINDER || obj->type == CONE)
	{
		if (!is_within_obj(hit_point, obj))
			return (-1);
	}
	info->distance = root;
	info->hit_point = hit_point;
	info->norm_vec = get_normal_vector(ray.direction, hit_point, obj);
	if (obj->surface != DEFAULT)
		info->color = get_point_color(obj, hit_point);
	else
		info->color = obj->color;
	info->obj_orient = obj->orient;
	info->obj_pos = obj->pos;
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

static t_color	get_point_color(t_obj_info *obj, t_vec hit_point)
{
	double	uv[2];
	t_color	color;
	int		flag;

	get_uv_mapping(obj, hit_point, uv);
	/* color = get_color_from_texture(obj, uv); */
	/* color = add_color(apply_bright(set_color(0.8, 0.8, 0.1), uv[0]), \ */
	/*         apply_bright(set_color(0.1, 0.4, 0.9), uv[1]));            */
	flag = (int)(floor(uv[0]) + floor(uv[1]));
	if (flag % 2)
		color = set_color(0, 0, 0);
	else
		color = set_color(1, 1, 1);
	return (color);
}

// U, V in range [0, 1]
// plane -> UV coordinate
// sphere -> UV mapping
// cylinder -> theta, h
// cone -> r (distance from top_point), theta on the
static void	get_uv_mapping(t_obj_info *obj, t_vec hit_point, double uv[2])
{
	static double	x_axis[3] = {1, 0, 0};
	static double	y_axis[3] = {0, 1, 0};
	const t_vec		x_vec = vec_make(x_axis);
	const t_vec		y_vec = vec_make(y_axis);
	t_vec			u_unit;
	t_vec			v_unit;
	t_vec			d;

	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		if (is_zero(fabs(obj->orient.y) - 1) == FALSE)
		{
			u_unit = vec_normalize(vec_crossprod(y_vec, obj->orient));
			v_unit = vec_normalize(vec_crossprod(obj->orient, u_unit));
		}
		else
		{
			v_unit = vec_normalize(vec_crossprod(x_vec, obj->orient));
			u_unit = vec_normalize(vec_crossprod(obj->orient, v_unit));
		}
		uv[0] = vec_dotprod(vec_minus(hit_point, obj->pos), u_unit);
		uv[1] = vec_dotprod(vec_minus(hit_point, obj->pos), v_unit);
	}
	else if (obj->type == SPHERE)
	{
		d = vec_normalize(vec_minus(obj->pos, hit_point));
		uv[0] = obj->radius * 2 * (0.5 + atan2(d.x, d.z) / (2.0 * M_PI));
		uv[1] = obj->radius * 2 * (0.5 + asin(d.y) / M_PI);
	}
	else if (obj->type == CYLINDER)
	{
		if (is_zero(fabs(obj->orient.y) - 1) == FALSE)
		{
			u_unit = vec_normalize(vec_crossprod(y_vec, obj->orient));
		}
		else
		{
			v_unit = vec_normalize(vec_crossprod(x_vec, obj->orient));
			u_unit = vec_normalize(vec_crossprod(obj->orient, v_unit));
		}
		uv[0] = obj->radius * 2.0 * acos(vec_dotprod(vec_normalize(vec_minus(vec_minus(hit_point, obj->pos), vec_scale(obj->orient, vec_dotprod((vec_minus(hit_point, obj->pos)), obj->orient)))), u_unit)) / M_PI;
		uv[1] = vec_dotprod(obj->orient, vec_minus(hit_point, obj->pos));
	}
	else if (obj->type == CONE)
	{
		if (is_zero(fabs(obj->orient.y) - 1) == FALSE)
		{
			u_unit = vec_normalize(vec_crossprod(y_vec, obj->orient));
		}
		else
		{
			v_unit = vec_normalize(vec_crossprod(x_vec, obj->orient));
			u_unit = vec_normalize(vec_crossprod(obj->orient, v_unit));
		}
		uv[0] = obj->radius * 2.0 * acos(vec_dotprod(vec_normalize(vec_minus(vec_minus(hit_point, obj->pos), vec_scale(obj->orient, vec_dotprod((vec_minus(hit_point, obj->pos)), obj->orient)))), u_unit)) / M_PI;
		uv[1] = 1.0 - vec_dotprod(vec_minus(hit_point, obj->pos), obj->orient);
	}
	else
	{
		return ;
	}
}
