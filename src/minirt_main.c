/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:50:09 by seseo             #+#    #+#             */
/*   Updated: 2022/07/19 16:33:25 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_var(t_vars *vars, char *arg)
{
	ft_memset(vars, 0, sizeof(t_vars));
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, 1920, 1080, "miniRT");
	vars->img.img = mlx_new_image(vars->mlx, 1920, 1080);
	vars->img.addr = mlx_get_data_addr(vars->img.img, \
		&vars->img.bits_per_pixel, &vars->img.line_length, &vars->img.endian);
	vars->map = map_parsing(arg);
}

int	draw_loop(t_vars *vars)
{
	(void)vars;
	return (0);
}

void	free_map(t_map *map)
{
	free(map->light);
	free(map->obj);
}

// close button press event
int	exit_hook(t_vars *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	free_map(vars->map);
	exit(0);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 2)
		return (EXIT_FAILURE);
	init_var(&vars, argv[1]);
	// mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_hook(vars.win, KEY_EVENT_EXIT, 0, &exit_hook, &vars);
	mlx_loop_hook(vars.mlx, draw_loop, &vars);
	mlx_loop(vars.mlx);
	return (0);
}
