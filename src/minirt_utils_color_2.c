/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils_color_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 21:10:07 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:26:27 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	set_color(double red, double green, double blue)
{
	t_color	color;

	color.r = red;
	color.g = green;
	color.b = blue;
	return (color);
}

t_color	set_color_from_int(int color_int)
{
	t_color	color;

	color.r = ((color_int >> 16) & 0xFF) / 255.0;
	color.g = ((color_int >> 8) & 0xFF) / 255.0;
	color.b = (color_int & 0xFF) / 255.0;
	return (color);
}

t_color	add_color(t_color c1, t_color c2)
{
	t_color	color;

	color.r = c1.r + c2.r;
	color.g = c1.g + c2.g;
	color.b = c1.b + c2.b;
	return (color);
}

t_color	color_mul(t_color color, double scale)
{
	t_color	ret;

	ret.r = color.r * scale;
	ret.g = color.g * scale;
	ret.b = color.b * scale;
	return (ret);
}
