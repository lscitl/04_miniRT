/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:50:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 21:45:41 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "minirt_hit.h"

void	set_map_info(t_vars *vars, t_map *map)
{
	set_ambi_light_and_light_info(vars, map);
	set_cam_info(vars->map, map->camera);
	set_obj_info(vars->map, map->obj);
	print_map_info(vars->map);
}

void	init_var_and_set_map_data(t_vars *vars, char *arg)
{
	t_map	*map;

	ft_memset(vars, 0, sizeof(t_vars));
	vars->map = ft_malloc(sizeof(t_map_info));
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, SCRN_WIDTH, SCRN_HEIGHT, "miniRT");
	vars->img.img = mlx_new_image(vars->mlx, SCRN_WIDTH, SCRN_HEIGHT);
	vars->img.addr = mlx_get_data_addr(vars->img.img, \
		&vars->img.bits_per_pixel, &vars->img.line_length, &vars->img.endian);
	map = map_parsing(arg);
	set_map_info(vars, map);
	free_map(map);
}

int	draw_loop(t_vars *vars)
{
	(void)vars;
	return (0);
}

// close button press event
int	exit_hook(t_vars *vars)
{
	mlx_destroy_image(vars->mlx, vars->img.img);
	mlx_destroy_window(vars->mlx, vars->win);
	free_map_info(vars->map);
	exit(EXIT_SUCCESS);
}

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_image(vars->mlx, vars->img.img);
		mlx_destroy_window(vars->mlx, vars->win);
		free_map_info(vars->map);
		exit(EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 2)
		return (EXIT_FAILURE);
	init_var_and_set_map_data(&vars, argv[1]);
	mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_hook(vars.win, KEY_EVENT_EXIT, 0, &exit_hook, &vars);
	draw_image(&vars, vars.map, vars.map->cam);
	// mlx_loop_hook(vars.mlx, draw_loop, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
