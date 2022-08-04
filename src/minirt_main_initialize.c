/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_initialize.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 22:46:22 by seseo             #+#    #+#             */
/*   Updated: 2022/08/04 19:43:12 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "minirt_hit.h"

static void	set_map_info(t_vars *vars, t_map *map);

void	init_var_and_set_map_data(t_vars *vars, char *arg)
{
	t_map	*map;

	ft_memset(vars, 0, sizeof(t_vars));
	vars->map = ft_malloc(sizeof(t_map_info));
	vars->mlx = mlx_init();
	if (vars->mlx == NULL)
		exit(EXIT_FAILURE);
	vars->win = mlx_new_window(vars->mlx, SCRN_WIDTH, SCRN_HEIGHT, "miniRT");
	vars->img.img = mlx_new_image(vars->mlx, SCRN_WIDTH, SCRN_HEIGHT);
	if (vars->img.img == NULL)
		exit(EXIT_FAILURE);
	vars->img.addr = mlx_get_data_addr(vars->img.img, \
		&vars->img.bits_per_pixel, &vars->img.line_length, &vars->img.endian);
	if (vars->win == NULL || vars->img.addr == NULL)
		exit(EXIT_FAILURE);
	map = map_parsing(arg);
	set_map_info(vars, map);
	free(map);
}

static void	set_map_info(t_vars *vars, t_map *map)
{
	set_ambi_light_and_light_info(vars->map, map, map->ambi_light);
	ft_lst_node_clear((t_list *)map->light);
	set_cam_info(vars->map, map->camera);
	ft_lst_node_clear((t_list *)map->camera);
	set_obj_info(vars->map, map->obj);
	ft_lst_node_clear((t_list *)map->obj);
}
