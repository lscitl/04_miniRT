/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_util_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 19:58:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/01 21:14:30 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	get_color(int color, int target_color)
{
	return ((color >> (target_color * 8)) & 0xFF);
}

t_color	set_color(double red, double green, double blue)
{
	t_color	color;

	color.r = red;
	color.g = green;
	color.b = blue;
	return (color);
}

t_color	add_color(t_color c1, t_color c2, t_color c3)
{
	t_color	color;

	color.r = c1.r + c2.r + c3.r;
	color.g = c1.g + c2.g + c3.g;
	color.b = c1.b + c2.b + c3.b;
	// if (color.r > 1)
	// 	color.r = 1;
	// if (color.g > 1)
	// 	color.g = 1;
	// if (color.b > 1)
	// 	color.b = 1;
	return (color);
}

t_color	apply_bright(t_color light, double bright)
{
	t_color	color;

	color.r = light.r * bright;
	color.g = light.g * bright;
	color.b = light.b * bright;
	return (color);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	free_map(t_map *map)
{
	ft_lst_node_clear((t_list *)map->light);
	ft_lst_node_clear((t_list *)map->camera);
	ft_lst_node_clear((t_list *)map->obj);
}

void	free_map_info(t_map_info *map)
{
	free(map->light);
	free(map->cam);
	free(map->obj);
	free(map);
}
