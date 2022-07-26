/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chanhpar <chanhpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 03:38:53 by chanhpar          #+#    #+#             */
/*   Updated: 2022/07/26 16:54:51 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_hit_plane(t_ray ray, t_obj *obj, t_hit_info info);
static int	is_hit_sphere(t_ray ray, t_obj *obj, t_hit_info info);
static int	is_hit_cylinder(t_ray ray, t_obj *obj, t_hit_info info);
static int	is_hit_cone(t_ray ray, t_obj *obj, t_hit_info info);
static int	is_hit_background(t_ray ray, t_obj *obj, t_hit_info info);

int	solve_quadratic(double coeff[3], double root[2]);
int	solve_linear(double coeff[3], double root[2]);

double	vec_dotprod(t_vec vec1, t_vec vec2);
t_vec	vec_crossprod(t_vec vec1, t_vec vec2);
double	vec_cos(t_vec vec1, t_vec vec2);
double	vec_sin(t_vec vec1, t_vec vec2);
t_vec	vec_normalize(t_vec vec);
double	vec_length(t_vec vec);
t_vec	vec_plus(t_vec vec1, t_vec vec2);
t_vec	vec_minus(t_vec vec1, t_vec vec2);
t_vec	vec_scale(t_vec vec1, double scale);

int	is_zero(double value);

// is_hit functions {{{

// hit return 0. no hit -> return -1.
// set hit information in info
int	is_hit(t_ray ray, t_obj *obj, t_hit_info info)
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
	else if (obj->type == cone)
	{
		return (is_hit_cone(ray, obj, info));
	}
	else
	{
		return (is_hit_background(ray, obj, info)); // or return -1 ?
	}
}

// (orig + t * dir - obj->pos) * orient == 0;
// what if direction vector is zero vector?
int	is_hit_plane(t_ray ray, t_obj *obj, t_hit_info info)
{
	double	coeff[3];
	double	root[3];
	int		flag;

	coeff[0] = vec_dotprod(ray.direction, vec_make(obj->orient));
	coeff[1] = vec_dotprod(vec_minus(ray.orig - vec_make(obj->pos)), vec_make(obj->orient));
	flag = solve_linear(coeff, root);
	if (flag == 0 && root[0] <= info.t && root[0] >= 0) // ray hit the plane at single point
	{
		info.t = root[0];
		info.norm_vec = vec_make(obj->orient);
		// info.color = get_color(obj->color);
		info.color = 0xFFFFFF;
		info.hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info.t));
		return (0);
	}
	else if (flag > 0 && info.t < 0) // ray is on the plane
	{
		info.t = 0;
		info.norm_vec = vec_make(obj->orient);
		// info.color = get_color(obj->color);
		info.color = 0xFFFFFF;
		info.hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info.t));
		return (0);
	}
	else
		return (-1); // does not update hit_info
}

// vec_length(orig + t * dir - obj->pos) == obj->radius
// what if direction vector is zero vector?
int	is_hit_sphere(t_ray ray, t_obj *obj, t_hit_info info)
{
	double	coeff[3];
	double	root[3];
	double	temp;
	int		flag;

	temp = vec_minus(ray.orig, vec_make(obj->pos));
	coeff[0] = vec_dotprod(ray.direction, ray.direction);
	coeff[1] = (double)2 * vec_dotprod(ray.direction, temp);
	coeff[2] = vec_dotprod(temp, temp) - (obj->radius * obj->radius);
	flag = solve_quadratic(coeff, root);
	if (flag >= 0) // ray hit the sphere at two points
	{
		if (root[0] <= info.t && root[0] >= 0) // check if root[0] is valid
		{
			info.t = root[0];
			info.hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info.t));
			info.norm_vec = vec_normalize(vec_minus(info.hit_point, vec_make(obj->pos)));
			// info.color = get_color(obj->color);
			info.color = 0xFFFFFF;
			return (0);
		}
		if (flag > 0 && root[1] <= info.t && root[1] >= 0) // check if root[1] is valid
		{
			info.t = root[1];
			info.hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info.t));
			info.norm_vec = vec_normalize(vec_minus(info.hit_point, vec_make(obj->pos)));
			// info.color = get_color(obj->color);
			info.color = 0xFFFFFF;
			return (0);
		}
	}
	return (-1); // does not update hit_info
}

// }}}

// solve equation {{{

// solve equation ax^2 + bx + c = 0
// coeff[0] = a (a != 0), coeff[1] = b, coeff[2] = c
// if D > 0, return 1. D = 0, return 0. D < 0, return -1.
// set root[0] = x1, root[1] = x2 (x1 < x2)
int	solve_quadratic(double coeff[3], double root[2])
{
	double	det;
	double	sqrt_det;

	det = coeff[1] * coeff[1] - 4 * coeff[0] * coeff[2];
	if (det < 0)
		return (-1);
	sqrt_det = sqrt(det);
	root[0] = (-coeff[1] - sqrt_det) / (2 * coeff[0]);
	root[1] = (-coeff[1] + sqrt_det) / (2 * coeff[0]);
	if (is_zero(det));
		return (0);
	return (1);
}

// solve equation ax + b = 0
// coeff[0] = a, coeff[1] = b
// if infinite root, return 1. one root, return 0. no root, return -1;
// set root[0] = x
// coeff[2] & root[1] is dummy to user same prototype with solve_quadratic
int	solve_linear(double coeff[3], double root[2])
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

// }}}

// t_vec utils {{{

t_vec	vec_make(double value[3])
{
	t_vec	result;

	result.x = value[0];
	result.y = value[1];
	result.z = value[2];
	return (result);
}

double	vec_dotprod(t_vec vec1, t_vec vec2)
{
	double	result;

	result = 0;
	result += vec1.x * vec2.x;
	result += vec1.y * vec2.y;
	result += vec1.z * vec2.z;
	return (result);
}

t_vec	vec_crossprod(t_vec vec1, t_vec vec2)
{
	t_vec	result;

	result.x = vec1.y * vec2.z - vec1.z * vec2.y;
	result.y = vec1.z * vec2.x - vec1.x * vec2.z;
	result.z = vec1.x * vec2.y - vec1.y * vec2.z;
	return (result);
}

double	vec_length(t_vec vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

t_vec	vec_normalize(t_vec vec)
{
	t_vec	result;
	double	length = vec_length(vec); // if length == 0?

	result.x = vec.x / length;
	result.y = vec.y / length;
	result.z = vec.z / length;
	return (result);
}

double	vec_sin(t_vec vec1, t_vec vec2)
{
	t_vec	norm_1;
	t_vec	norm_2;

	norm_1 = vec_normalize(vec1);
	norm_2 = vec_normalize(vec2);
	return (vec_length(vec_crossprod(norm_1, norm_2)));
}

double	vec_cos(t_vec vec1, t_vec vec2)
{
	return (vec_dotprod(vec_normalize(vec1), vec_normalize(vec2)));
}

t_vec	vec_plus(t_vec vec1, t_vec vec2)
{
	t_vec	result;

	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	result.z = vec1.z + vec2.z;
	return (result);
}

t_vec	vec_minus(t_vec vec1, t_vec vec2)
{
	t_vec	result;

	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	result.z = vec1.z - vec2.z;
	return (result);
}

t_vec	vec_scale(t_vec vec1, double scale)
{
	t_vec	result;

	result.x = vec1.x * scale;
	result.y = vec1.y * scale;
	result.z = vec1.z * scale;
	return (result);
}

// }}}

int	is_zero(double value)
{
	return (value <= __FLT_EPSILON__ && -value <= __FLT_EPSILON__);
}
