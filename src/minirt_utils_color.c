/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils_color.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 21:10:07 by seseo             #+#    #+#             */
/*   Updated: 2022/08/03 20:29:41 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_color(int color, int target_color)
{
	return ((color >> (target_color * 8)) & 0xFF);
}

int	convert_color(t_color color)
{
	int	red;
	int	green;
	int	blue;

	red = (int)color.r;
	green = (int)color.g;
	blue = (int)color.b;
	return (red << 16 | green << 8 | blue);
}

t_color	set_color(double red, double green, double blue)
{
	t_color	color;

	color.r = red;
	color.g = green;
	color.b = blue;
	return (color);
}

t_color	set_color2(int color_int)
{
	t_color	color;

	color.r = (color_int >> 16) & 0xFF;
	color.g = (color_int >> 8) & 0xFF;
	color.b = color_int & 0xFF;
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

// t_color	add_color(t_color c1, t_color c2, t_color c3)
// {
// 	t_color	color;

// 	color.r = c1.r + c2.r + c3.r;
// 	color.g = c1.g + c2.g + c3.g;
// 	color.b = c1.b + c2.b + c3.b;
// 	return (color);
// }

t_color	apply_bright(t_color light, double bright)
{
	t_color	color;

	color.r = light.r * bright;
	color.g = light.g * bright;
	color.b = light.b * bright;
	return (color);
}
