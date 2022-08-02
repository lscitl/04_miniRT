/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 03:38:53 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/02 20:46:57 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>
#include "minirt.h"
#include "minirt_hit.h"

static int	is_hit_plane(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_sphere(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cylinder(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_circle(t_ray ray, t_obj_info *obj, t_hit_info *info);
static int	is_hit_cone(t_ray ray, t_obj_info *obj, t_hit_info *info);

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
