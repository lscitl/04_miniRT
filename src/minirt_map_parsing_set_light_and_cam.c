/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_light_and_cam.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 17:01:59 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 23:30:47 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_pos_and_color_light(t_light *light, char **pos, char **color);
static int	set_pos_and_orient_cam(t_camera *cam, char **pos, char **orient);

int	set_ambient_light(t_map *map, char **args)
{
	char	**color;
	char	*end;
	int		i;

	if (args_len_check(args, 3))
		return (EXIT_FAILURE);
	map->ambi_light.bright = ft_strtod(args[1], &end);
	if (*end != 0 || map->ambi_light.bright < 0 || map->ambi_light.bright > 1)
		return (EXIT_FAILURE);
	color = ft_split(args[2], ',');
	if (args_len_check(color, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (color[i])
	{
		map->ambi_light.color[i] = ft_strtod(color[i], &end);
		if (*end != 0)
			return (EXIT_FAILURE);
		i++;
	}
	if (color_check(map->ambi_light.color))
		return (EXIT_FAILURE);
	free_strs(color);
	return (EXIT_SUCCESS);
}

int	set_light(t_map *map, char **args)
{
	t_light	*light;
	char	**pos;
	char	**color;
	char	*end;

	if (args_len_check(args, 3) && args_len_check(args, 4))
		return (EXIT_FAILURE);
	light = ft_malloc(sizeof(t_light));
	ft_memset(light, 0, sizeof(t_light));
	light->bright = ft_strtod(args[2], &end);
	if (*end != 0)
		return (EXIT_FAILURE);
	pos = ft_split(args[1], ',');
	color = NULL;
	if (args[3])
		color = ft_split(args[3], ',');
	if (set_pos_and_color_light(light, pos, color)
		|| color_check(light->color))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(color);
	ft_lightadd_back(&map->light, light);
	return (EXIT_SUCCESS);
}

static int	set_pos_and_color_light(t_light *light, char **pos, char **color)
{
	char	*pos_end;
	char	*color_end;
	int		i;

	if (args_len_check(pos, 3) || (color && args_len_check(color, 3)))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		light->pos[i] = ft_strtod(pos[i], &pos_end);
		if (color)
			light->color[i] = (int)ft_strtod(color[i], &color_end);
		else
			light->color[i] = 255;
		if (*pos_end != 0 || (color && *color_end != 0))
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	set_camera(t_map *map, char **args)
{
	t_camera	*cam;
	char		**pos;
	char		**orient;
	char		*end;

	if (args_len_check(args, 4))
		return (EXIT_FAILURE);
	cam = ft_malloc(sizeof(t_camera));
	ft_memset(cam, 0, sizeof(t_camera));
	cam->angle = (int)ft_strtod(args[3], &end);
	if (*end != 0 || cam->angle <= 0 || cam->angle >= 180)
		return (EXIT_FAILURE);
	pos = ft_split(args[1], ',');
	orient = ft_split(args[2], ',');
	if (set_pos_and_orient_cam(cam, pos, orient) || orient_check(cam->orient))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(orient);
	ft_camadd_back(&map->camera, cam);
	return (EXIT_SUCCESS);
}

static int	set_pos_and_orient_cam(t_camera *cam, char **pos, char **orient)
{
	int			i;
	char		*pos_end;
	char		*orient_end;

	if (args_len_check(pos, 3) || args_len_check(orient, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (pos[i] && orient[i])
	{
		cam->pos[i] = ft_strtod(pos[i], &pos_end);
		cam->orient[i] = ft_strtod(orient[i], &orient_end);
		if (*pos_end != 0 || *orient_end != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
