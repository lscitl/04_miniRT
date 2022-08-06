/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_set_cam.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 17:01:59 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 18:19:29 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static int	set_camera_list(t_camera *cam, t_map *map, char **args);
static int	set_pos_and_orient_cam(t_camera *cam, char **pos, char **orient);

int	set_camera(t_map *map, char **args)
{
	t_camera	*cam;
	char		*end;

	if (get_args_len(args) != 4)
		return (EXIT_FAILURE);
	cam = ft_malloc(sizeof(t_camera));
	ft_memset(cam, 0, sizeof(t_camera));
	cam->angle = ft_strtod(args[3], &end);
	if (*end != 0 || cam->angle <= 0 || cam->angle >= 180)
	{
		free(cam);
		return (EXIT_FAILURE);
	}
	return (set_camera_list(cam, map, args));
}

static int	set_camera_list(t_camera *cam, t_map *map, char **args)
{
	char		**pos;
	char		**orient;
	int			exit_flag;

	pos = ft_split(args[1], ',');
	orient = ft_split(args[2], ',');
	exit_flag = (set_pos_and_orient_cam(cam, pos, orient) \
			|| orient_check(cam->orient));
	free_strs(pos);
	free_strs(orient);
	if (exit_flag == EXIT_FAILURE)
		free(cam);
	else
		ft_lstadd_back((t_list **)(&map->camera), (t_list *)cam);
	return (exit_flag);
}

static int	set_pos_and_orient_cam(t_camera *cam, char **pos, char **orient)
{
	int			i;
	char		*pos_end;
	char		*orient_end;

	if (get_args_len(pos) != 3 || get_args_len(orient) != 3)
		return (EXIT_FAILURE);
	i = 0;
	while (pos[i] && orient[i])
	{
		cam->pos[i] = ft_strtod(pos[i], &pos_end);
		cam->orient[i] = ft_strtod(orient[i], &orient_end);
		if (*pos_end != 0 || *orient_end != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
