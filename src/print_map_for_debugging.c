/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map_for_debugging.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 17:22:14 by seseo             #+#    #+#             */
/*   Updated: 2022/07/27 19:35:13 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_strs(char **strs)
{
	while (*strs)
		printf("%s\n", *strs++);
}

void	print_pos(double pos[3])
{
	printf("\tpos: %f, %f, %f\n", pos[0], pos[1], pos[2]);
}

void	print_orient(double orient[3])
{
	printf("\torient: %f, %f, %f\n", orient[0], orient[1], orient[2]);
}

void	print_color(int color)
{
	printf("\tcolor: %d, %d, %d\n", get_color(color, RED), \
										get_color(color, GREEN), \
										get_color(color, BLUE));
}

void	print_camera(t_camera *cam)
{
	int	i;

	i = 0;
	while (cam)
	{
		printf("cam_idx %d\n", i);
		print_pos(cam->pos);
		print_orient(cam->orient);
		printf("\tdegree %f\n", cam->angle);
		i++;
		cam = cam->next;
		printf("\n");
	}
}

void	print_light(t_light *light)
{
	int	i;

	i = 0;
	while (light)
	{
		printf("light_idx %d\n", i);
		print_pos(light->pos);
		printf("\tbright %f\n", light->bright);
		print_color(light->color);
		i++;
		light = light->next;
		printf("\n");
	}
}

void	print_plane(t_obj *obj)
{
	printf("\tobj type: PLANE\n");
	print_pos(obj->pos);
	print_orient(obj->orient);
	print_color(obj->color);
}

void	print_sphere(t_obj *obj)
{
	printf("\tobj type: SPHERE\n");
	print_pos(obj->pos);
	printf("\tdiameter %f\n", obj->diameter);
	print_color(obj->color);
}

void	print_cylinder(t_obj *obj)
{
	printf("\tobj type: CYLINDER\n");
	print_pos(obj->pos);
	print_orient(obj->orient);
	printf("\tdiameter %f\n", obj->diameter);
	printf("\theight %f\n", obj->height);
	print_color(obj->color);
}

void	print_cone(t_obj *obj)
{
	printf("\tobj type: CONE\n");
	print_pos(obj->pos);
	print_orient(obj->orient);
	printf("\tdiameter %f\n", obj->diameter);
	printf("\theight %f\n", obj->height);
	print_color(obj->color);
}

void	print_obj(t_obj *obj)
{
	int	i;

	i = 0;
	while (obj)
	{
		printf("obj_idx %d\n", i);
		if (obj->type == PLANE)
			print_plane(obj);
		else if (obj->type == SPHERE)
			print_sphere(obj);
		else if (obj->type == CYLINDER)
			print_cylinder(obj);
		else
			print_cone(obj);
		i++;
		obj = obj->next;
		printf("\n");
	}
}

void	print_ambi_light(t_ambi_light *ambi_light)
{
	printf("Ambi_light\n\tbirght: %f\n", ambi_light->bright);
	print_color(ambi_light->color);
	printf("\n");
}

void	print_map(t_map *map)
{
	printf("\n");
	print_ambi_light(&map->ambi_light);
	print_camera(map->camera);
	print_light(map->light);
	print_obj(map->obj);
}

void	print_map_info(t_map_info *map)
{
	int	i;

	printf("Ambi_light\n");
	printf("\tbright : %f\n", map->ambi_light.bright);
	printf("\tcolor : %f, %f, %f\n", map->ambi_light.color.r, map->ambi_light.color.g, map->ambi_light.color.b);
	i = 0;
	while (i < map->cam_cnt)
	{
		printf("Camera idx %d\n", i);
		printf("\tpos : %f, %f, %f\n", map->cam[i].pos.x, map->cam[i].pos.y, map->cam[i].pos.z);
		printf("\torient : %f, %f, %f\n", map->cam[i].orient.x, map->cam[i].orient.y, map->cam[i].orient.z);
		printf("\tangle : %f\n", map->cam[i].angle / M_PI * 180);
		printf("\tscreen : %f, %f, %f\n", map->cam[i].screen.x, map->cam[i].screen.y, map->cam[i].screen.z);
		printf("\tfocal_len : %f\n\n", map->cam[i].focal_len);
		i++;
	}
	i = 0;
	while (i < map->light_cnt)
	{
		printf("Light idx %d\n", i);
		printf("\tpos : %f, %f, %f\n", map->light[i].pos.x, map->light[i].pos.y, map->light[i].pos.z);
		printf("\tbright : %f\n", map->light[i].bright);
		printf("\tcolor : %f, %f, %f\n\n", map->light[i].color.r, map->light[i].color.g, map->light[i].color.b);
		i++;
	}
	i = 0;
	while (i < map->obj_cnt)
	{
		printf("Object idx %d\n", i);
		printf("\tobj_type : %d\n", map->obj[i].type);
		printf("\tpos : %f, %f, %f\n", map->obj[i].pos.x, map->obj[i].pos.y, map->obj[i].pos.z);
		printf("\torient : %f, %f, %f\n", map->obj[i].orient.x, map->obj[i].orient.y, map->obj[i].orient.z);
		printf("\tcolor : %f, %f, %f\n", map->obj[i].color.r, map->obj[i].color.g, map->obj[i].color.b);
		printf("\tradius : %f\n", map->obj[i].radius);
		printf("\theight : %f\n", map->obj[i].height);
		printf("\twidth : %f\n\n", map->obj[i].width);
		i++;
	}
}
