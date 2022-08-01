/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_math_vec_utils_3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:00:24 by seseo             #+#    #+#             */
/*   Updated: 2022/08/01 13:00:11 by seseo            ###   ########.fr       */
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
