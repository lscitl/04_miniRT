/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_obj.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:10:55 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:16:25 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_obj_type(char *identifier);
static int	set_plane(t_map *map, t_obj *plane, char **args);
static int	set_sphere(t_map *map, t_obj *sphere, char **args);
static int	set_cylinder_and_cone(t_map *map, t_obj *obj, char **args);

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

static int	set_plane(t_map *map, t_obj *plane, char **args)
{
	int		args_len;

	args_len = get_args_len(args++);
	if (args_len < 4 || set_obj_pos(plane, args++) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (set_obj_orient(plane, args++) == EXIT_FAILURE || \
		set_obj_surface_and_color(plane, args) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_lstadd_back((t_list **)(&map->obj), (t_list *)plane);
	return (EXIT_SUCCESS);
}

static int	set_sphere(t_map *map, t_obj *sphere, char **args)
{
	char	*end;
	int		args_len;

	args_len = get_args_len(args++);
	if (args_len < 4 || set_obj_pos(sphere, args++) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	sphere->diameter = ft_strtod(*args++, &end);
	if (*end || sphere->diameter <= EPSILON)
		return (EXIT_FAILURE);
	if (set_obj_surface_and_color(sphere, args) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_lstadd_back((t_list **)(&map->obj), (t_list *)sphere);
	return (EXIT_SUCCESS);
}

static int	set_cylinder_and_cone(t_map *map, t_obj *obj, char **args)
{
	char	*d_end;
	char	*h_end;
	int		args_len;

	args_len = get_args_len(args++);
	if (args_len < 6 || set_obj_pos(obj, args++) == EXIT_FAILURE || \
		set_obj_orient(obj, args++) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	obj->diameter = ft_strtod(*args++, &d_end);
	obj->height = ft_strtod(*args++, &h_end);
	if (*d_end || *h_end || obj->diameter <= EPSILON || obj->height <= EPSILON \
		|| set_obj_surface_and_color(obj, args) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_lstadd_back((t_list **)(&map->obj), (t_list *)obj);
	return (EXIT_SUCCESS);
}
