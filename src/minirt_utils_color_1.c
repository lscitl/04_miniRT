/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils_color_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 21:10:07 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:26:33 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_color(int color, int target_color)
{
	return ((color >> (target_color * 8)) & 0xFF);
}

int	convert_color_to_int(t_color color)
{
	int	red;
	int	green;
	int	blue;

	red = (int)color.r;
	green = (int)color.g;
	blue = (int)color.b;
	return (red << 16 | green << 8 | blue);
}
