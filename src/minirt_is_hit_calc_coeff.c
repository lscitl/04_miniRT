/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_calc_coeff.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chanhpar <chanhpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 16:28:31 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/03 16:43:50 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static void	calc_coeff_sphere(double coeff[3], t_ray ray, t_obj_info *obj);
static void	calc_coeff_cylinder(double coeff[3], t_ray ray, t_obj_info *obj);
static void	calc_coeff_cone(double coeff[3], t_ray ray, t_obj_info *obj);
static void	calc_coeff_plane(double coeff[3], t_ray ray, t_obj_info *obj);

void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj)
{
	if (obj->type == SPHERE)
	{
		calc_coeff_sphere(coeff, ray, obj);
	}
	else if (obj->type == CYLINDER)
	{
		calc_coeff_cylinder(coeff, ray, obj);
	}
	else if (obj->type == CONE)
	{
		calc_coeff_cone(coeff, ray, obj);
	}
	else
	{
		calc_coeff_plane(coeff, ray, obj);
	}
}

static void	calc_coeff_sphere(double coeff[3], t_ray ray, t_obj_info *obj)
{
	t_vec	w;

	w = vec_minus(ray.orig, obj->pos);
	coeff[0] = vec_dotprod(ray.direction, ray.direction);
	coeff[1] = (double)2 * vec_dotprod(ray.direction, w);
	coeff[2] = vec_dotprod(w, w) - (obj->r_sqare);
}

static void	calc_coeff_cylinder(double coeff[3], t_ray ray, t_obj_info *obj)
{
	t_vec	v;
	t_vec	w;
	t_vec	h;
	double	v_dot_h;
	double	w_dot_h;

	h = obj->orient;
	v = ray.direction;
	w = vec_minus(ray.orig, obj->pos);
	v_dot_h = vec_dotprod(v, h);
	w_dot_h = vec_dotprod(w, h);
	coeff[0] = 1.0 - pow(v_dot_h, 2);
	coeff[1] = 2.0 * (vec_dotprod(v, w) - v_dot_h * w_dot_h);
	coeff[2] = vec_dotprod(w, w) - pow(w_dot_h, 2) - obj->r_sqare;
}

static void	calc_coeff_cone(double coeff[3], t_ray ray, t_obj_info *obj)
{
	t_vec	v;
	t_vec	w;
	t_vec	h;
	double	v_dot_h;
	double	w_dot_h;

	h = obj->orient;
	v = ray.direction;
	w = vec_minus(ray.orig, obj->pos);
	v_dot_h = vec_dotprod(v, h);
	w_dot_h = vec_dotprod(w, h);
	coeff[0] = 1.0 - (1.0 + obj->rsq_div_hsq) * pow(v_dot_h, 2);
	coeff[1] = 2.0 * (vec_dotprod(w, v) + obj->rsq_div_h * v_dot_h \
			- (1.0 + obj->rsq_div_hsq) * w_dot_h * v_dot_h);
	coeff[2] = vec_dotprod(w, w) - (1.0 + obj->rsq_div_hsq) * pow(w_dot_h, 2) \
			- obj->r_sqare + 2.0 * obj->rsq_div_h * w_dot_h;
}

static void	calc_coeff_plane(double coeff[3], t_ray ray, t_obj_info *obj)
{
	coeff[0] = vec_dotprod(ray.direction, obj->orient);
	coeff[1] = vec_dotprod(vec_minus(ray.orig, obj->pos), obj->orient);
}
