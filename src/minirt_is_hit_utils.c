/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chanhpar <chanhpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:03:47 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/02 20:28:52 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);

int	update_hit_info(t_vec dir, t_hit_info *info, t_obj_info *obj, t_vec hit_point, double root)
{
	info->distance = root;
	info->hit_point = hit_point;
	info->norm_vec = get_normal_vector(dir, hit_point, obj);
	info->color = obj->color;
	return (0);
}

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj)
{
	t_vec	n;

	if (obj->type == SPHERE)
	{
		n = (vec_normalize(vec_minus(point, obj->pos)));
	}
	else if (obj->type == CYLINDER)
	{
		n = (vec_normalize(vec_crossprod(obj->orient, vec_crossprod(vec_minus(point, obj->pos), obj->orient))));
	}
	else if (obj->type == CONE)
	{
		n = (vec_normalize(vec_crossprod(vec_minus(vec_plus(obj->pos, vec_scale(obj->orient, obj->height)), point), vec_crossprod(vec_minus(point, obj->pos), obj->orient))));
	}
	else
	{
		n = (obj->orient);
	}
	if (vec_dotprod(n, dir) > 0)
		return (vec_scale(n, (double)-1));
	return (n);
}

int	is_within_obj(t_vec intersect, t_obj_info *obj)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, obj->pos), obj->orient);
	return (projection >= -EPSILON && projection <= obj->height);
}

void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj)
{
	t_vec	v;
	t_vec	w;
	t_vec	h;
	double	v_dot_h;
	double	w_dot_h;
	double	w_dot_v;
	// double	cos_square_theta;

	h = obj->orient;
	v = ray.direction;
	w = vec_minus(ray.orig, obj->pos);
	if (obj->type == SPHERE)
	{
		coeff[0] = vec_dotprod(ray.direction, ray.direction);
		coeff[1] = (double)2 * vec_dotprod(ray.direction, w);
		coeff[2] = vec_dotprod(w, w) - (obj->r_sqare);
	}
	else if (obj->type == CYLINDER)
	{
		v_dot_h = vec_dotprod(v, h);
		w_dot_h = vec_dotprod(w, h);
		coeff[0] = 1.0 - pow(v_dot_h, 2);
		coeff[1] = 2.0 * (vec_dotprod(v, w) - v_dot_h * w_dot_h);
		coeff[2] = vec_dotprod(w, w) - pow(w_dot_h, 2) - obj->r_sqare;
	}
	else if (obj->type == CONE)
	{
		v_dot_h = vec_dotprod(v, h);
		w_dot_h = vec_dotprod(w, h);
		w_dot_v = vec_dotprod(w, v);
		coeff[0] = 1.0 - (1.0 + obj->rsq_div_hsq) * pow(v_dot_h, 2);
		coeff[1] = 2.0 * (vec_dotprod(w, v) + obj->rsq_div_h * v_dot_h - (1.0 + obj->rsq_div_hsq) * w_dot_h * v_dot_h);
		coeff[2] = vec_dotprod(w, w) - (1.0 + obj->rsq_div_hsq) * pow(w_dot_h, 2) - obj->r_sqare + 2.0 * obj->rsq_div_h * w_dot_h;
	}
	else
	{
		coeff[0] = vec_dotprod(ray.direction, obj->orient);
		coeff[1] = vec_dotprod(vec_minus(ray.orig, obj->pos), obj->orient);
	}
}

