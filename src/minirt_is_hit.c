/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 03:38:53 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/02 20:01:59 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>
#include "minirt.h"

static int	is_hit_plane(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_sphere(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cylinder(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_circle(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cone(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_within_obj(t_vec intersect, t_obj_info *obj);

static int	update_hit_info(t_vec dir, t_hit_info *info, t_obj_info *obj, t_vec hit_point, double root);
static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);
static void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj);

int			solve_quadratic(double coeff[3], double root[2]);
int			solve_linear(double coeff[3], double root[2]);

int			is_zero(double value);

// is_hit functions {{{

// if hit_info is updated, return 0. no hit -> return -1.
// set hit information in info
int	is_hit(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	if (obj->type == PLANE)
	{
		return (is_hit_plane(ray, obj, info));
	}
	else if (obj->type == SPHERE)
	{
		return (is_hit_sphere(ray, obj, info));
	}
	else if (obj->type == CYLINDER)
	{
		return (is_hit_cylinder(ray, obj, info));
	}
	else if (obj->type == CONE)
	{
		return (is_hit_cone(ray, obj, info));
	}
	else if (obj->type == CIRCLE)
	{
		return (is_hit_circle(ray, obj, info));
	}
	else
	{
		return (-1);
	}
}

// (orig + t * dir - obj->pos) * orient == 0;
// what if direction vector is zero vector?
static int	is_hit_plane(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	double	coeff[3];
	double	root[3];
	int		flag;

	calc_coeff(coeff, ray, obj);
	flag = solve_linear(coeff, root);
	if (flag > 0)
		root[0] = 0;
	if (flag >= 0 && root[0] <= info->distance && root[0] >= 0)
	{
		return (update_hit_info(ray.direction, info, obj, vec_ray_at_distance(ray, root[0]), root[0]));
	}
	else
		return (-1);
}

// vec_length(orig + t * dir - obj->pos) == obj->radius
static int	is_hit_sphere(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	double	coeff[3];
	double	root[3];
	int		flag;

	calc_coeff(coeff, ray, obj);
	flag = solve_quadratic(coeff, root);
	if (flag >= 0) // ray hit the sphere at two points
	{
		if (root[0] <= info->distance && root[0] >= 0) // check if root[0] is valid
		{
			return (update_hit_info(ray.direction, info, obj, vec_ray_at_distance(ray, root[0]), root[0]));
		}
		if (flag > 0 && root[1] <= info->distance && root[1] >= 0) // check if root[1] is valid
		{
			return (update_hit_info(ray.direction, info, obj, vec_ray_at_distance(ray, root[1]), root[1]));
		}
	}
	return (-1); // does not update hit_info
}

static int	is_hit_cylinder(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	t_vec	intersect;
	double	coeff[3];
	double	root[2];
	int		flag;

	calc_coeff(coeff, ray, obj);
	if (is_zero(coeff[0]))
		return (-1);
	flag = solve_quadratic(coeff, root);
	if (flag >= 0) // ray hit the cylinder at two points
	{
		if (root[0] <= info->distance && root[0] >= 0) // check if root[0] is valid
		{
			intersect = vec_ray_at_distance(ray, root[0]);
			if (is_within_obj(intersect, obj))
			{
				return (update_hit_info(ray.direction, info, obj, intersect, root[0]));
			}
		}
		if (flag > 0 && root[1] <= info->distance && root[1] >= 0) // check if root[1] is valid
		{
			intersect = vec_ray_at_distance(ray, root[1]);
			if (is_within_obj(intersect, obj))
			{
				return (update_hit_info(ray.direction, info, obj, intersect, root[1]));
			}
		}
	}
	return (-1);
}

static int	is_hit_circle(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	t_hit_info	temp;
	t_vec		dist;
	int			flag;

	temp = *info;
	//temp.distance = info->distance;
	flag = is_hit_plane(ray, obj, &temp);
	if (flag < 0)
		return (-1);
	dist = vec_minus(temp.hit_point, obj->pos);
	if (vec_dotprod(dist, dist) > obj->r_sqare)
		return (-1);
	*info = temp;
	return (0);
}

// (vec_dotprod(u, d) - vec_length(u) * vec_length(d) * cos(theta) == 0)
// u = ray.orig + t * ray.direction - obj->pos
// (temp + t * dir) * d = | temp + t * dir | * | d | * costheta
static int	is_hit_cone(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	t_vec	intersect;
	double	coeff[3];
	double	root[2];
	int		flag;

	calc_coeff(coeff, ray, obj);
	if (is_zero(coeff[0]))
		return (-1); // XXX
	flag = solve_quadratic(coeff, root);
	if (flag > 0) // ray hit the cone at two points
	{
		if (root[0] <= info->distance && root[0] >= 0) // check if root[0] is valid
		{
			intersect = vec_ray_at_distance(ray, root[0]);
			if (is_within_obj(intersect, obj))
			{
				return (update_hit_info(ray.direction, info, obj, intersect, root[0]));
			}
		}
		if (flag > 0 && root[1] <= info->distance && root[1] >= 0) // check if root[1] is valid
		{
			intersect = vec_ray_at_distance(ray, root[1]);
			if (is_within_obj(intersect, obj))
			{
				return (update_hit_info(ray.direction, info, obj, intersect, root[1]));
			}
		}
	}
	return (-1);
}

static int	update_hit_info(t_vec dir, t_hit_info *info, t_obj_info *obj, t_vec hit_point, double root)
{
	info->distance = root;
	info->hit_point = hit_point;
	info->norm_vec = get_normal_vector(dir, hit_point, obj);
	info->color = obj->color;
	// printf("%f, %f, %f\n", info->color.r, info->color.g, info->color.b);
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

static int	is_within_obj(t_vec intersect, t_obj_info *obj)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, obj->pos), obj->orient);
	return (projection >= -EPSILON && projection <= obj->height);
}

static void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj)
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
		// cos_square_theta = (obj->height * obj->height);
		// cos_square_theta /= (obj->height * obj->height + obj->radius * obj->radius);
		// coeff[0] = vec_dotprod(v, h) * vec_dotprod(v, h) - cos_square_theta;
		// coeff[1] = 2 * (vec_dotprod(v, h) * vec_dotprod(w, h) - cos_square_theta * vec_dotprod(w, v));
		// coeff[2] = vec_dotprod(w, h) * vec_dotprod(w, h) - cos_square_theta * vec_dotprod(w, w);
		coeff[0] = 1.0 - (1.0 + obj->rsq_div_hsq) * pow(v_dot_h, 2);
		coeff[1] = 2.0 * (vec_dotprod(w, v) + obj->rsq_div_h * v_dot_h - (1.0 + obj->rsq_div_hsq) * w_dot_h * v_dot_h);
		coeff[2] = vec_dotprod(w, w) - (1.0 + obj->rsq_div_hsq) * pow(w_dot_h, 2) - obj->r_sqare + 2.0 * obj->rsq_div_h * w_dot_h;
		// coeff[0] = 1.0 - (1.0 + obj->rsq_div_hsq) * v_dot_h * v_dot_h;
		// coeff[1] = 2.0 * (w_dot_v - (1.0 + obj->rsq_div_hsq) * v_dot_h * w_dot_h);
		// coeff[2] = vec_dotprod(w, w) - (1.0 + obj->rsq_div_hsq) * w_dot_h * w_dot_h;
	}
	else
	{
		coeff[0] = vec_dotprod(ray.direction, obj->orient);
		coeff[1] = vec_dotprod(vec_minus(ray.orig, obj->pos), obj->orient);
	}
}

// }}}

// solve equation {{{

// solve equation ax^2 + bx + c = 0
// coeff[0] = a (a != 0), coeff[1] = b, coeff[2] = c
// if D > 0, return 1. D = 0, return 0. D < 0, return -1.
// set root[0] = x1, root[1] = x2 (x1 < x2)
int	solve_quadratic(double *coeff, double *root)
{
	double	det;
	double	sqrt_det;

	det = coeff[1] * coeff[1] - 4 * coeff[0] * coeff[2];
	if (det < 0)
		return (-1);
	sqrt_det = sqrt(det);
	root[0] = (-coeff[1] - sqrt_det) / (2 * coeff[0]);
	root[1] = (-coeff[1] + sqrt_det) / (2 * coeff[0]);
	if (is_zero(det))
		return (0);
	return (1);
}

// solve equation ax + b = 0
// coeff[0] = a, coeff[1] = b
// if infinite root, return 1. one root, return 0. no root, return -1;
// set root[0] = x
// coeff[2] & root[1] is dummy to user same prototype with solve_quadratic
int	solve_linear(double *coeff, double *root)
{
	if (is_zero(coeff[0]))
	{
		if (is_zero(coeff[1]))
			return (1);
		else
			return (-1);
	}
	root[0] = -coeff[1] / coeff[0];
	return (0);
}