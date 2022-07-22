/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj_sphere.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/07/22 16:27:00 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_pos_and_color_sphere(t_obj *sphere, char **pos, char **color);

int	set_sphere(t_map *map, t_obj *sphere, char **args)
{
	char	**pos;
	char	**color;
	char	*end;

	if (args_len_check(args, 4))
		return (EXIT_FAILURE);
	sphere->diameter = strtod(args[2], &end);
	if (*end || sphere->diameter <= __FLT_EPSILON__)
		return (EXIT_FAILURE);
	pos = ft_split(args[1], ',');
	color = ft_split(args[3], ',');
	if (set_pos_and_color_sphere(sphere, pos, color)
		|| color_check(sphere->color))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(color);
	ft_objadd_back(&map->obj, sphere);
	return (EXIT_SUCCESS);
}

static int	set_pos_and_color_sphere(t_obj *sphere, char **pos, char **color)
{
	char	*pos_end;
	char	*color_end;
	int		i;

	if (args_len_check(pos, 3) || args_len_check(color, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		sphere->pos[i] = ft_strtod(pos[i], &pos_end);
		sphere->color[i] = (int)ft_strtod(color[i], &color_end);
		if (*pos_end != 0 || *color_end != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
