/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils_obj_2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 20:56:04 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 13:26:19 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	set_cb(t_obj *obj, char **args)
{
	char	*end;

	if (args[1])
	{
		obj->surface = CHECKER_BOARD;
		obj->map_scale = strtod(args[1], &end);
		if (*end)
			return (EXIT_FAILURE);
		if (args[2])
			return (set_phong(obj, &args[2]));
		return (set_phong_default(obj));
	}
	return (EXIT_FAILURE);
}

int	set_bm(t_obj *obj, char **args)
{
	if (args[1])
	{
		obj->surface |= BUMP_MAP;
		if (ft_strncmp(args[1] + ft_strlen(args[1]) - 4, ".xpm", -1) == 0)
			obj->bm = ft_strdup(args[1]);
		else
		{
			ft_putstr_fd("img file is not .xpm\n", 2);
			return (EXIT_FAILURE);
		}
		if (args[2])
		{
			if (ft_strncmp(args[2], "tx", -1) == 0)
				return (set_tx(obj, &args[2]));
			return (set_phong(obj, &args[2]));
		}
		return (set_phong_default(obj));
	}
	return (EXIT_FAILURE);
}

int	set_tx(t_obj *obj, char **args)
{
	if (args[1])
	{
		obj->surface |= TEXTURE;
		if (ft_strncmp(args[1] + ft_strlen(args[1]) - 4, ".xpm", -1) == 0)
			obj->tx = ft_strdup(args[1]);
		else
			return (EXIT_FAILURE);
		if (args[2])
			return (set_phong(obj, &args[2]));
		return (set_phong_default(obj));
	}
	return (EXIT_FAILURE);
}

int	set_phong(t_obj *obj, char **args)
{
	char	*end;
	int		args_len;

	args_len = get_args_len(args);
	if (args_len != 3)
		return (EXIT_FAILURE);
	obj->kd = ft_strtod(args[0], &end);
	if (*end != 0)
		return (EXIT_FAILURE);
	obj->ks = ft_strtod(args[1], &end);
	if (*end != 0)
		return (EXIT_FAILURE);
	obj->alpha = ft_strtod(args[2], &end);
	if (*end != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	set_phong_default(t_obj *obj)
{
	if (obj == NULL)
		return (EXIT_FAILURE);
	obj->kd = 1.0;
	obj->ks = 0.5;
	obj->alpha = 64;
	return (EXIT_SUCCESS);
}
