/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_hit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 03:38:53 by chanhpar          #+#    #+#             */
/*   Updated: 2022/07/28 01:28:51 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	is_hit_plane(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_sphere(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cylinder(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_circle(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cone(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_within_obj(t_vec intersect, t_vec center, t_vec height);
/* static int	is_hit_background(t_ray ray, t_obj_info *obj, t_hit_info *info); */

int			solve_quadratic(double coeff[3], double root[2]);
int			solve_linear(double coeff[3], double root[2]);

double		vec_dotprod(t_vec vec1, t_vec vec2);
t_vec		vec_crossprod(t_vec vec1, t_vec vec2);
double		vec_cos(t_vec vec1, t_vec vec2);
double		vec_sin(t_vec vec1, t_vec vec2);
t_vec		vec_normalize(t_vec vec);
double		vec_length(t_vec vec);
t_vec		vec_plus(t_vec vec1, t_vec vec2);
t_vec		vec_minus(t_vec vec1, t_vec vec2);
t_vec		vec_scale(t_vec vec1, double scale);

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
		/* return (is_hit_background(ray, obj, info)); // or return -1 ? */
	}
}

// (orig + t * dir - obj->pos) * orient == 0;
// what if direction vector is zero vector?
static int	is_hit_plane(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	double	coeff[3];
	double	root[3];
	int		flag;

	coeff[0] = vec_dotprod(ray.direction, obj->orient);
	coeff[1] = vec_dotprod(vec_minus(ray.orig, obj->pos), obj->orient);
	flag = solve_linear(coeff, root);
	if (flag == 0 && root[0] <= info->t && root[0] >= 0) // ray hit the plane at single point
	{
		info->t = root[0];
		info->norm_vec = obj->orient;
		info->color = obj->color;
		info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
		return (0);
	}
	else if (flag > 0 && info->t < 0) // ray is on the plane
	{
		info->t = 0;
		info->norm_vec = obj->orient;
		info->color = obj->color;
		info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
		return (0);
	}
	else
		return (-1); // does not update hit_info
}

// vec_length(orig + t * dir - obj->pos) == obj->radius
static int	is_hit_sphere(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	double	coeff[3];
	double	root[3];
	t_vec	temp;
	int		flag;

	temp = vec_minus(ray.orig, obj->pos);
	coeff[0] = vec_dotprod(ray.direction, ray.direction);
	coeff[1] = (double)2 * vec_dotprod(ray.direction, temp);
	coeff[2] = vec_dotprod(temp, temp) - (obj->r_sqare);
	flag = solve_quadratic(coeff, root);
	if (flag >= 0) // ray hit the sphere at two points
	{
		if (root[0] <= info->t && root[0] >= 0) // check if root[0] is valid
		{
			info->t = root[0];
			info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
			info->norm_vec = vec_normalize(vec_minus(info->hit_point, obj->pos));
			info->color = obj->color;
			return (0);
		}
		if (flag > 0 && root[1] <= info->t && root[1] >= 0) // check if root[1] is valid
		{
			info->t = root[1];
			info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
			info->norm_vec = vec_normalize(vec_minus(info->hit_point, obj->pos));
			info->color = obj->color;
			return (0);
		}
	}
	return (-1); // does not update hit_info
}

static int	is_hit_cylinder(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	t_vec	v;
	t_vec	w;
	t_vec	h;
	t_vec	intersect;
	double	coeff[3];
	double	root[2];
	int		flag;

	v = ray.direction;
	w = vec_minus(ray.orig, obj->pos);
	h = obj->orient;
	coeff[0] = vec_dotprod(v, v) - vec_dotprod(v, h) * vec_dotprod(v, h);
	coeff[1] = 2 * vec_dotprod(v, w) - vec_dotprod(v, h) * vec_dotprod(w, h);
	coeff[2] = vec_dotprod(w, w) - vec_dotprod(w, h) * vec_dotprod(w, h) - obj->r_sqare;
	if (is_zero(coeff[0]))
		return (-1);
	flag = solve_quadratic(coeff, root);
	if (flag > 0) // ray hit the cylinder at two points
	{
		if (root[0] <= info->t && root[0] >= 0) // check if root[0] is valid
		{
			intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[0]));
			if (is_within_obj(intersect, obj->pos, obj->orient))
			{
				info->t = root[0];
				info->hit_point = intersect;
				info->norm_vec = vec_normalize(vec_crossprod(obj->orient, vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
				info->color = obj->color;
				return (0);
			}
		}
		if (root[1] <= info->t && root[1] >= 0) // check if root[1] is valid
		{
			intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[1]));
			if (is_within_obj(intersect, obj->pos, obj->orient))
			{
				info->t = root[1];
				info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
				info->norm_vec = vec_normalize(vec_crossprod(obj->orient, vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
				info->color = obj->color;
				return (0);
			}
		}
	}
	if (flag == 0)
	{
		intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[0]));
		if (is_within_obj(intersect, obj->pos, obj->orient))
		{
			info->t = root[0];
			info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
			info->norm_vec = vec_normalize(vec_crossprod(obj->orient, vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
			info->color = obj->color;
			return (0);
		}
	}
	return (-1);
}

static int	is_hit_circle(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	t_hit_info	temp;
	t_vec		dist;

	is_hit_plane(ray, obj, &temp);
	dist = vec_minus(temp.hit_point, obj->pos);
	if (vec_dotprod(dist, dist) > obj->radius)
		return (-1);
	*info = temp;
	return (0);
}

// (vec_dotprod(u, d) - vec_length(u) * vec_length(d) * cos(theta) == 0)
// u = ray.orig + t * ray.direction - (obj->pos + obj->orient * obj->height)
// (temp + t * dir) * d = | temp + t * dir | * | d | * costheta
static int	is_hit_cone(t_ray ray, t_obj_info *obj, t_hit_info *info)
{
	double	cos_square_theta;
	t_vec	d;
	t_vec	v;
	t_vec	u;
	t_vec	intersect;
	double	coeff[3];
	double	root[2];
	int		flag;

	d = obj->orient;
	v = ray.direction;
	u = vec_minus(ray.orig, vec_plus(obj->pos, vec_scale(obj->orient, obj->height)));
	cos_square_theta = (obj->height * obj->height);
	cos_square_theta /= (obj->height * obj->height + obj->radius + obj->radius);
	coeff[0] = vec_dotprod(v, d) * vec_dotprod(v, d) - cos_square_theta;
	coeff[1] = 2 * (vec_dotprod(v, d) * vec_dotprod(u, d) - cos_square_theta * vec_dotprod(u, v));
	coeff[2] = vec_dotprod(u, d) * vec_dotprod(u, d) - cos_square_theta * vec_dotprod(u, u);
	if (is_zero(coeff[0]))
		return (-1); // XXX
	flag = solve_quadratic(coeff, root);
	if (flag > 0) // ray hit the cone at two points
	{
		if (root[0] <= info->t && root[0] >= 0) // check if root[0] is valid
		{
			intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[0]));
			if (is_within_obj(intersect, obj->pos, obj->orient))
			{
				info->t = root[0];
				info->hit_point = intersect;
				info->norm_vec = vec_normalize(vec_crossprod(vec_minus(vec_plus(obj->pos, vec_scale(obj->orient, obj->height)), intersect), vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
				info->color = obj->color;
				return (0);
			}
		}
		if (root[1] <= info->t && root[1] >= 0) // check if root[1] is valid
		{
			intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[1]));
			if (is_within_obj(intersect, obj->pos, obj->orient))
			{
				info->t = root[1];
				info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
				info->norm_vec = vec_normalize(vec_crossprod(vec_minus(vec_plus(obj->pos, vec_scale(obj->orient, obj->height)), intersect), vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
				info->color = obj->color;
				return (0);
			}
		}
	}
	if (flag == 0)
	{
		intersect = vec_plus(ray.orig, vec_scale(ray.direction, root[0]));
		if (is_within_obj(intersect, obj->pos, obj->orient))
		{
			info->t = root[0];
			info->hit_point = vec_plus(ray.orig, vec_scale(ray.direction, info->t));
			info->norm_vec = vec_normalize(vec_crossprod(vec_minus(vec_plus(obj->pos, vec_scale(obj->orient, obj->height)), intersect), vec_crossprod(vec_minus(info->hit_point, obj->pos), obj->orient)));
			info->color = obj->color;
			return (0);
		}
	}
	return (-1);
}

static int	is_within_obj(t_vec intersect, t_vec center, t_vec height)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, center), height);
	return (projection >= 0 && projection <= 1);
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
