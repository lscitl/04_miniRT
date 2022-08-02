/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_map_data.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 15:17:21 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 14:18:08 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_params(t_map *map, char **args);

int	set_map_data(t_map *map, char **map_args)
{
	char	**args;
	int		i;

	i = 0;
	while (map_args[i])
	{
		replace_all_white_space_to_sp(map_args[i]);
		args = ft_split(map_args[i], ' ');
		if (set_params(map, args))
			return (EXIT_FAILURE);
		free_strs(args);
		free(map_args[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	set_params(t_map *map, char **args)
{
	if (ft_strncmp(*args, "A", -1) == 0)
		return (set_ambient_light(map, args));
	else if (ft_strncmp(*args, "C", -1) == 0)
		return (set_camera(map, args));
	else if (ft_strncmp(args[0], "L", -1) == 0)
		return (set_light(map, args));
	else if (!ft_strncmp(args[0], "pl", -1) || !ft_strncmp(args[0], "sp", -1) \
			|| !ft_strncmp(args[0], "cy", -1) || !ft_strncmp(args[0], "co", -1))
		return (set_obj(map, args));
	else if (ft_strncmp(*args, "#", 1) == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
