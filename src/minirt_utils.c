/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 19:58:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 22:10:17 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	free(map);
}

void	free_map_info(t_map_info *map)
{
	free(map->light);
	free(map->cam);
	free(map->obj);
	free(map);
}
