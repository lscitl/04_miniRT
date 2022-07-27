/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj_sphere.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/07/26 18:56:51 by seseo            ###   ########.fr       */
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
	if (set_pos_and_color_sphere(sphere, pos, color))
		return (EXIT_FAILURE);
	free_strs(pos);
	free_strs(color);
	ft_lstadd_back((t_list **)(&map->obj), (t_list *)sphere);
	return (EXIT_SUCCESS);
}

static int	set_pos_and_color_sphere(t_obj *sphere, char **pos, char **color)
{
	char	*pos_end;
	int		color_tmp;
	int		i;

	if (args_len_check(pos, 3) || args_len_check(color, 3))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		sphere->pos[i] = ft_strtod(pos[i], &pos_end);
		if (*pos_end != 0 && is_all_digit(color[i]))
			return (EXIT_FAILURE);
		color_tmp = ft_atoi(color[i]);
		if (color_tmp < 0 || color_tmp > 255)
			return (EXIT_FAILURE);
		sphere->color |= color_tmp << (2 - i) * 8;
		// sphere->color = sphere->color << 8 | color_tmp;
		i++;
	}
	return (EXIT_SUCCESS);
}
