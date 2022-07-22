/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj_plane.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 23:26:29 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_pos_orient_color(t_obj *obj, char **pos, \
									char **orient, char **color);

int	set_plane(t_map *map, t_obj *plane, char **args)
{
	char	**pos;
	char	**orient;
	char	**color;

	if (args_len_check(args, 4))
		return (EXIT_FAILURE);
	pos = ft_split(args[1], ',');
	orient = ft_split(args[2], ',');
	color = ft_split(args[3], ',');
	if (set_pos_orient_color(plane, pos, orient, color)
		|| color_check(plane->color) || orient_check(plane->orient))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(orient);
	free_strs(color);
	ft_objadd_back(&map->obj, plane);
	return (EXIT_SUCCESS);
}

static int	set_pos_orient_color(t_obj *obj, char **pos, \
									char **orient, char **color)
{
	char	*pos_end;
	char	*orient_end;
	char	*color_end;
	int		i;

	if (args_len_check(pos, 3) || args_len_check(orient, 3)
		|| args_len_check(color, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		obj->pos[i] = ft_strtod(pos[i], &pos_end);
		obj->orient[i] = ft_strtod(orient[i], &orient_end);
		obj->color[i] = (int)ft_strtod(color[i], &color_end);
		if (*pos_end != 0 || *orient_end != 0 || *color_end != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
