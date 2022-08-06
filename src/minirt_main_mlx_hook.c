/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_mlx_hook.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 22:46:24 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 20:52:17 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	static int	i;

	if (keycode == KEY_ESC)
	{
		mlx_destroy_image(vars->mlx, vars->img.img);
		mlx_destroy_window(vars->mlx, vars->win);
		free_map_info(vars->map);
		exit(EXIT_SUCCESS);
	}
	if (keycode == KEY_1)
	{
		if (vars->map->cam_cnt > 1)
		{
			i = (i + 1) % (vars->map->cam_cnt);
			printf("Change to Cam index %d\n", i);
			vars->map->cam_idx = i;
			draw_image(vars, vars->map, &vars->map->cam[i]);
		}
	}
	return (EXIT_SUCCESS);
}
