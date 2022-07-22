/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 22:21:42 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_obj_type(char *identifier);

int	set_obj(t_map *map, char **args)
{
	t_obj	*obj;
	int		obj_type;

	obj_type = set_obj_type(*args);
	if (obj_type == NONE)
		return (EXIT_FAILURE);
	obj = ft_malloc(sizeof(t_obj));
	ft_memset(obj, 0, sizeof(t_obj));
	obj->type = obj_type;
	if (obj_type == PLANE)
		return (set_plane(map, obj, args));
	else if (obj_type == SPHERE)
		return (set_sphere(map, obj, args));
	else
		return (set_cylinder_and_cone(map, obj, args));
}

static int	set_obj_type(char *identifier)
{
	if (ft_strncmp(identifier, "pl", -1) == 0)
		return (PLANE);
	else if (ft_strncmp(identifier, "sp", -1) == 0)
		return (SPHERE);
	else if (ft_strncmp(identifier, "cy", -1) == 0)
		return (CYLINDER);
	else if (ft_strncmp(identifier, "co", -1) == 0)
		return (CONE);
	else
		return (NONE);
}
