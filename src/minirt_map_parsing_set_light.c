/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_light.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 17:01:59 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 18:21:57 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_pos_and_color_light(t_light *light, char **pos, char **color);
static int	set_light_list(t_light *light, t_map *map, char **args);

int	set_ambient_light(t_map *map, char **args)
{
	char	**color;
	char	*end;
	int		color_tmp;
	int		i;

	if (get_args_len(args) != 3)
		return (EXIT_FAILURE);
	map->ambi_light.bright = ft_strtod(args[1], &end);
	if (*end != 0 || map->ambi_light.bright < 0 || map->ambi_light.bright > 1)
		return (EXIT_FAILURE);
	color = ft_split(args[2], ',');
	if (get_args_len(color) != 3)
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		color_tmp = ft_atoi(color[i]);
		if (is_all_digit(color[i]) || color_tmp < 0 || color_tmp > 255)
			return (EXIT_FAILURE);
		map->ambi_light.color |= color_tmp << (2 - i) * 8;
		i++;
	}
	free_strs(color);
	return (EXIT_SUCCESS);
}

int	set_light(t_map *map, char **args)
{
	t_light	*light;
	char	*end;

	if (get_args_len(args) != 3 && get_args_len(args) != 4)
		return (EXIT_FAILURE);
	light = ft_malloc(sizeof(t_light));
	ft_memset(light, 0, sizeof(t_light));
	light->bright = ft_strtod(args[2], &end);
	if (*end != 0)
	{
		free(light);
		return (EXIT_FAILURE);
	}
	return (set_light_list(light, map, args));
}

static int	set_light_list(t_light *light, t_map *map, char **args)
{
	char	**pos;
	char	**color;
	int		exit_flag;

	pos = ft_split(args[1], ',');
	color = NULL;
	if (args[3])
		color = ft_split(args[3], ',');
	exit_flag = set_pos_and_color_light(light, pos, color);
	free_strs(pos);
	free_strs(color);
	if (exit_flag == EXIT_FAILURE)
		free(light);
	else
		ft_lstadd_back((t_list **)(&map->light), (t_list *)light);
	return (exit_flag);
}

static int	set_pos_and_color_light(t_light *light, char **pos, char **color)
{
	char	*pos_end;
	int		color_tmp;
	int		i;

	if (get_args_len(pos) != 3 || (color && get_args_len(color) != 3))
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		light->pos[i] = ft_strtod(pos[i], &pos_end);
		if (*pos_end != 0 || is_all_digit(color[i]))
			return (EXIT_FAILURE);
		if (color)
		{
			color_tmp = ft_atoi(color[i]);
			if (color_tmp < 0 || color_tmp > 255)
				return (EXIT_FAILURE);
		}
		else
			color_tmp = 255;
		light->color |= color_tmp << (2 - i) * 8;
		i++;
	}
	return (EXIT_SUCCESS);
}
