/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:50:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 22:50:08 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "minirt_hit.h"

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 2)
		return (EXIT_FAILURE);
	init_var_and_set_map_data(&vars, argv[1]);
	mlx_key_hook(vars.win, &key_hook, &vars);
	mlx_hook(vars.win, KEY_EVENT_EXIT, 0, &exit_hook, &vars);
	draw_image(&vars, vars.map, vars.map->cam);
	mlx_loop(vars.mlx);
	return (0);
}
