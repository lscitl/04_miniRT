/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_phong_and_shadow.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:53:53 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 20:58:53 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static double	adjust_bright(double bright, double light_len);
static void		apply_diffuse_and_specular(t_hit_info *info, \
								t_light_info light, t_vec v, t_phong *phong);
static double	is_in_shadow(t_map_info *map, t_hit_info *info, \
													t_vec light_pos);

/*
color of hit point = ambient +
				(diffuse + specular)(for all light sources)
Ip = ka * ia + sigma[from 0 to light_cnt - 1]
				(kd * (Lm dotprod N) * im,d + ks * (Rm dotprod V) ^ alpha * im,s)
Rm = 2 * (Lm dotprod N) * N - Lm
*/
t_phong	phong_reflection(t_map_info *map, t_hit_info *info, t_vec v)
{
	int			light_index;

	info->phong.ka = map->ambi_light.bright;
	info->phong.ambient = color_mul(map->ambi_light.color, info->phong.ka);
	info->phong.point_color = info->phong.ambient;
	info->phong.specular = set_color(0, 0, 0);
	light_index = 0;
	while (light_index < map->light_cnt)
	{
		info->phong.light_len = \
			is_in_shadow(map, info, map->light[light_index].pos);
		if (info->phong.light_len)
			apply_diffuse_and_specular(info, map->light[light_index], \
															v, &info->phong);
		light_index++;
	}
	return (info->phong);
}

static double	is_in_shadow(t_map_info *map, t_hit_info *info, t_vec light_pos)
{
	t_hit_info	sdw_info;
	double		vec_len;
	int			obj_index;

	sdw_info = *info;
	sdw_info.distance = DBL_MAX;
	sdw_info.ray.direction = \
		vec_normalize(vec_minus(light_pos, info->hit_point));
	sdw_info.ray.orig = \
		vec_plus(info->hit_point, vec_scale(sdw_info.ray.direction, EPSILON));
	vec_len = vec_length(vec_minus(light_pos, sdw_info.ray.orig)) + EPSILON;
	obj_index = 0;
	while (obj_index < map->obj_cnt)
	{
		if (is_hit(&map->obj[obj_index], &sdw_info) == 0)
		{
			if (sdw_info.distance <= vec_len && sdw_info.distance >= 0)
				return (0);
		}
		obj_index++;
	}
	return (vec_len);
}

static void	apply_diffuse_and_specular(t_hit_info *info, t_light_info light, \
													t_vec v, t_phong *phong)
{
	t_vec	light_direction;
	t_vec	reflect_direction;
	double	bright;
	double	l_dot_n;
	double	r_dot_v_alpha;

	bright = adjust_bright(light.bright, phong->light_len);
	light_direction = vec_normalize(vec_minus(light.pos, info->hit_point));
	phong->kd = fmax(vec_dotprod(info->norm_vec, light_direction), 0);
	phong->diffuse = color_mul(light.color, phong->kd * bright);
	l_dot_n = vec_dotprod(light_direction, info->norm_vec);
	reflect_direction = \
		vec_minus(vec_scale(info->norm_vec, 2.0 * l_dot_n), light_direction);
	r_dot_v_alpha = pow(fmax(vec_dotprod(reflect_direction, v), 0), \
			phong->alpha);
	phong->specular = add_color(phong->specular, color_mul(light.color, \
				phong->ks * r_dot_v_alpha * bright));
	phong->point_color = add_color(phong->point_color, phong->diffuse);
}

static double	adjust_bright(double bright, double light_len)
{
	if (light_len > LIGHT_LEN_LIMIT)
	{
		light_len /= LIGHT_LEN_LIMIT;
		return (bright / (light_len * light_len));
	}
	return (bright);
}
