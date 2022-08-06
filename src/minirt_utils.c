/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 19:58:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 17:54:14 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	get_mlx_pixel_color(t_data *data, int x, int y)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
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
	int	i;

	free(map->light);
	free(map->cam);
	i = 0;
	while (i < map->obj_cnt)
	{
		if (map->obj[i].surface & BUMP_MAP)
			mlx_destroy_image(map->obj[i].bm.mlx, map->obj[i].bm.img);
		if (map->obj[i].surface & TEXTURE)
			mlx_destroy_image(map->obj[i].tx.mlx, map->obj[i].tx.img);
		if (map->obj[i].type == CYLINDER)
			i += 2;
		else if (map->obj[i].type == CONE)
			i += 1;
		i++;
	}
	free(map->obj);
	free(map);
}

void	open_texture_img(t_data *img, char *file_name)
{
	img->mlx = mlx_init();
	if (img->mlx == NULL)
		exit(EXIT_FAILURE);
	img->img = mlx_xpm_file_to_image(img->mlx, file_name, &img->w, &img->h);
	if (img->img == NULL)
		exit(EXIT_FAILURE);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, \
									&img->line_length, &img->endian);
	if (img->addr == NULL)
		exit(EXIT_FAILURE);
}
