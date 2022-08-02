/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main_mlx_hook.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 22:46:24 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 22:48:32 by seseo            ###   ########.fr       */
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
	if (keycode == KEY_ESC)
	{
		mlx_destroy_image(vars->mlx, vars->img.img);
		mlx_destroy_window(vars->mlx, vars->win);
		free_map_info(vars->map);
		exit(EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
