/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_math_vec_utils_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 15:00:24 by seseo             #+#    #+#             */
/*   Updated: 2022/07/28 16:48:22 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

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
	result.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return (result);
}

double	vec_length(t_vec vec)
{
	return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

t_vec	vec_normalize(t_vec vec)
{
	t_vec			result;
	const double	length = vec_length(vec);

	if (is_zero(length))
		return (vec);
	result.x = vec.x / length;
	result.y = vec.y / length;
	result.z = vec.z / length;
	return (result);
}
