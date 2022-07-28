/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:50:09 by seseo             #+#    #+#             */
/*   Updated: 2022/07/29 01:16:53 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <float.h>
#include "minirt.h"

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
	mlx_destroy_window(vars->mlx, vars->win);
	free_map_info(vars->map);
	exit(EXIT_SUCCESS);
}

int	key_hook(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
	{
		mlx_destroy_window(vars->mlx, vars->win);
		free_map_info(vars->map);
		exit(EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

void	dummy(void)
{
}

void	test_draw(t_map_info *map)
{
	t_cam_info	*cam;
	t_hit_info	info;
	int			i;
	int			j;
	int			obj_index;
	t_ray		ray;

	cam = map->cam;
	ft_memset(&info, 0, sizeof(t_hit_info));
	fprintf(stderr, "P3\n%d %d\n255\n", SCRN_WIDTH, SCRN_HEIGHT);
	i = 0;
	while (i < SCRN_HEIGHT)
	{
		j = 0;
		while (j < SCRN_WIDTH)
		{
			info.distance = DBL_MAX;
			ray.orig = cam->pos;
			ray.direction = vec_normalize(vec_plus(vec_plus(cam->screen, \
							vec_scale(cam->x_vec, j)), \
						vec_scale(cam->y_vec, i)));
			obj_index = 0;
			if (i == SCRN_WIDTH / 2 && j == SCRN_HEIGHT / 2)
				dummy();
			/* fprintf(stderr, "%d %d %d\n", (int)(255.999 * i / (SCRN_HEIGHT - 1)), (int)(255.999 * j / (SCRN_WIDTH - 1)), 10); */
			while (obj_index < map->obj_cnt)
			{
				is_hit(ray, &map->obj[obj_index], &info);
				obj_index++;
			}
			if (info.distance < DBL_MAX)
				fprintf(stderr, "%d %d %d\n", (int)info.color.r, (int)info.color.g, (int)info.color.b);
			else
				fprintf(stderr, "%d %d %d\n", 0, 0, 0);
			j++;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 2)
		return (EXIT_FAILURE);
	init_var_and_set_map_data(&vars, argv[1]);
	test_draw(vars.map);
	// mlx_key_hook(vars.win, &key_hook, &vars);
	// mlx_hook(vars.win, KEY_EVENT_EXIT, 0, &exit_hook, &vars);
	// mlx_loop_hook(vars.mlx, draw_loop, &vars);
	// mlx_loop(vars.mlx);
	return (0);
}
