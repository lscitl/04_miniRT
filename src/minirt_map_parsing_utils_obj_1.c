/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils_obj_1.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 20:56:04 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:29:15 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	set_obj_pos(t_obj *obj, char **args)
{
	char	**pos;
	char	*pos_end;
	int		i;

	pos = ft_split(*args, ',');
	if (get_args_len(pos) != 3)
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		obj->pos[i] = ft_strtod(pos[i], &pos_end);
		if (*pos_end != 0)
		{
			free_strs(pos);
			return (EXIT_FAILURE);
		}
		i++;
	}
	free_strs(pos);
	return (EXIT_SUCCESS);
}

int	set_obj_orient(t_obj *obj, char **args)
{
	char	**orient;
	char	*orient_end;
	int		i;

	orient = ft_split(*args, ',');
	if (get_args_len(orient) != 3)
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		obj->orient[i] = ft_strtod(orient[i], &orient_end);
		if (*orient_end != 0)
		{
			free_strs(orient);
			return (EXIT_FAILURE);
		}
		i++;
	}
	free_strs(orient);
	if (orient_check(obj->orient))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	set_obj_default_surface_color(t_obj *obj, char **args)
{
	char	**color;
	int		color_tmp;
	int		i;

	color = ft_split(*args++, ',');
	if (get_args_len(color) != 3)
		return (EXIT_FAILURE);
	i = 0;
	while (i < 3)
	{
		color_tmp = ft_atoi(color[i]);
		if (is_all_digit(color[i]) || color_tmp < 0 || color_tmp > 255)
		{
			free_strs(color);
			return (EXIT_FAILURE);
		}
		obj->color |= color_tmp << (2 - i) * 8;
		i++;
	}
	free_strs(color);
	set_phong_default(obj);
	if (*args)
		if (set_phong(obj, args) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	set_obj_surface_and_color(t_obj *obj, char **args)
{
	if (ft_strncmp(*args, "bm", -1) == 0)
	{
		if (set_bm(obj, args) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (ft_strncmp(*args, "tx", -1) == 0)
	{
		if (set_tx(obj, args) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (ft_strncmp(*args, "cb", -1) == 0)
	{
		if (set_cb(obj, args) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		if (set_obj_default_surface_color(obj, args) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
