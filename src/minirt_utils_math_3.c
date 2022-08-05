/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils_math_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:00:24 by seseo             #+#    #+#             */
/*   Updated: 2022/08/05 15:25:57 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

int	is_zero(double value)
{
	return (fabs(value) <= EPSILON);
}

t_vec	vec_ray_at_distance(t_ray ray, double distance)
{
	return (vec_plus(ray.orig, vec_scale(ray.direction, distance)));
}

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

void	find_uv_unit_vec(t_vec orient, t_vec *u_unit, t_vec *v_unit)
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
