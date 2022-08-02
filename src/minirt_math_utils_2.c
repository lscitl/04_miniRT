/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_math_vec_utils_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:00:24 by seseo             #+#    #+#             */
/*   Updated: 2022/07/27 15:03:37 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

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
