/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj_plane.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/07/26 18:57:36 by seseo            ###   ########.fr       */
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
		|| orient_check(plane->orient))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(orient);
	free_strs(color);
	ft_lstadd_back((t_list **)(&map->obj), (t_list *)plane);
	return (EXIT_SUCCESS);
}

static int	set_pos_orient_color(t_obj *obj, char **pos, \
									char **orient, char **color)
{
	char	*pos_end;
	char	*orient_end;
	int		color_tmp;
	int		i;

	if (args_len_check(pos, 3) || args_len_check(orient, 3)
		|| args_len_check(color, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		obj->pos[i] = ft_strtod(pos[i], &pos_end);
		obj->orient[i] = ft_strtod(orient[i], &orient_end);
		if (*pos_end != 0 || *orient_end != 0 || is_all_digit(color[i]))
			return (EXIT_FAILURE);
		color_tmp = ft_atoi(color[i]);
		if (color_tmp < 0 || color_tmp > 255)
			return (EXIT_FAILURE);
		obj->color |= color_tmp << (2 - i) * 8;
		i++;
	}
	return (EXIT_SUCCESS);
}
