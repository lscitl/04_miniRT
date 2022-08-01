/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 15:50:09 by seseo             #+#    #+#             */
/*   Updated: 2022/08/01 21:32:08 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	set_shadow_flag(t_map_info *map, t_hit_info *info, int *flag)
{
	t_hit_info	shadow_info;
	t_ray		hit_point_to_light;
	int			obj_index;
	int			light_index;

	shadow_info.distance = DBL_MAX;
	light_index = 0;
	while (light_index < map->light_cnt)
	{
		// hit_point_to_light.orig = info->hit_point;
		hit_point_to_light.direction = \
			vec_normalize(vec_minus(map->light[light_index].pos, info->hit_point));
		hit_point_to_light.orig = vec_plus(info->hit_point, vec_scale(hit_point_to_light.direction, 0.1));
		obj_index = 0;
		while (obj_index < map->obj_cnt)
		{
			is_hit(hit_point_to_light, &map->obj[obj_index], &shadow_info);
			obj_index++;
		}
		if (shadow_info.distance < DBL_MAX && shadow_info.distance > -EPSILON)
		{
			printf("%f\n", shadow_info.distance);
			flag[light_index] = 1;
			break ;
		}
		light_index++;
	}
}

/*
color of hit point = ambient +
				(diffuse + specular)(zero to all light sources)
Ip = ka * ia + sigma(from 0 to light_cnt)
				(kd * (Lm dotprod N) * im,d + ks * (Rm dotprod V) ^ alpha * im,s)
Rm = 2 * (Lm dotprod N) * N - Lm
*/
t_color	phong_reflection(t_map_info *map, t_hit_info *info, t_vec v, int *flag)
{
	t_phong		param;
	t_color		point_color;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	t_vec		light_direction;
	t_vec		reflect_direction;
	int			light_index;

	param.ka = map->ambi_light.bright;
	param.ks = 0.5;
	param.alpha = 64;
	ambient = set_color(map->ambi_light.color.r, map->ambi_light.color.g, map->ambi_light.color.b);
	ambient = apply_bright(ambient, param.ka);
	point_color = ambient;
	light_index = 0;
	while (light_index < map->light_cnt)
	{
		set_shadow_flag(map, info, flag);
		if (flag[light_index] == 0)
		{
			light_direction = vec_normalize(vec_minus(map->light[light_index].pos, info->hit_point));
			param.kd = fmax(vec_dotprod(info->norm_vec, light_direction), 0);
			diffuse = apply_bright(apply_bright(map->light[light_index].color, param.kd), map->light[light_index].bright);
			reflect_direction = vec_minus(vec_scale(info->norm_vec, 2.0 * vec_dotprod(light_direction, info->norm_vec)), light_direction);
			// printf("%f, %f, %f, %f\n", reflect_direction.x, reflect_direction.y, reflect_direction.z, vec_length(reflect_direction));
			specular = apply_bright(apply_bright(map->light->color, param.ks * pow(vec_dotprod(reflect_direction, v), param.alpha)), map->light[light_index].bright);
			point_color = add_color(point_color, diffuse, specular);
			// point_color = add_color(point_color, diffuse, set_color(0, 0, 0));
		}
		light_index++;
	}
	return (point_color);
}

void	test_draw(t_map_info *map)
{
	t_cam_info	*cam;
	t_hit_info	info;
	int			i;
	int			j;
	int			obj_index;
	t_ray		ray;
	int			*shadow_flag;
	t_color		color;

	cam = map->cam;
	shadow_flag = ft_malloc(sizeof(int) * map->light_cnt);
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
			if (i == SCRN_WIDTH / 2 && j == SCRN_HEIGHT / 2)
				dummy();
			obj_index = 0;
			while (obj_index < map->obj_cnt)
			{
				is_hit(ray, &map->obj[obj_index], &info);
				obj_index++;
			}
			// if (info.distance < DBL_MAX)
			// {
			// }
			if (info.distance < DBL_MAX)
			{
				ft_memset(shadow_flag, 0, sizeof(int) * map->light_cnt);
				color = phong_reflection(map, &info, cam->orient_neg, shadow_flag);
				if (color.r > 1)
					color.r = 1;
				if (color.g > 1)
					color.g = 1;
				if (color.b > 1)
					color.b = 1;
				color.r = (color.r * info.color.r * 255);
				color.g = (color.g * info.color.g * 255);
				color.b = (color.b * info.color.b * 255);
				fprintf(stderr, "%d %d %d\n", (int)color.r, (int)color.g, (int)color.b);
			}
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
