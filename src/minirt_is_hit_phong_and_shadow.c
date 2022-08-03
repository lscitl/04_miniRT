/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_phong_and_shadow.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 15:53:53 by seseo             #+#    #+#             */
/*   Updated: 2022/08/03 16:30:55 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static double	adjust_bright(double bright, double light_len);
static void		apply_diffuse_and_specular(t_hit_info *info, \
								t_light_info light, t_vec v, t_phong *param);
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
	t_phong		param;
	int			light_index;

	param.ka = map->ambi_light.bright;
	param.ks = 0.5;
	param.alpha = 64;
	param.ambient = map->ambi_light.color;
	param.ambient = apply_bright(param.ambient, param.ka);
	param.point_color = param.ambient;
	param.specular = set_color(0, 0, 0);
	light_index = 0;
	while (light_index < map->light_cnt)
	{
		param.light_len = is_in_shadow(map, info, map->light[light_index].pos);
		if (param.light_len)
			apply_diffuse_and_specular(info, map->light[light_index], \
																v, &param);
		light_index++;
	}
	return (param);
}

static double	is_in_shadow(t_map_info *map, t_hit_info *info, t_vec light_pos)
{
	t_hit_info	shadow_info;
	t_ray		hit_point;
	t_vec		light;
	double		vec_len;
	int			obj_index;

	shadow_info.distance = DBL_MAX;
	hit_point.direction = \
		vec_normalize(vec_minus(light_pos, info->hit_point));
	hit_point.orig = \
		vec_plus(info->hit_point, vec_scale(hit_point.direction, EPSILON));
	obj_index = 0;
	while (obj_index < map->obj_cnt)
	{
		is_hit(hit_point, &map->obj[obj_index], &shadow_info);
		obj_index++;
	}
	light = light_pos;
	vec_len = vec_length(vec_minus(light, hit_point.orig));
	if (shadow_info.distance <= vec_len + EPSILON && shadow_info.distance >= 0)
		return (0);
	return (vec_len);
}

static void	apply_diffuse_and_specular(t_hit_info *info, t_light_info light, \
													t_vec v, t_phong *param)
{
	t_vec	light_direction;
	t_vec	reflect_direction;
	double	bright;
	double	l_dot_n;
	double	r_dot_v_alpha;

	bright = adjust_bright(light.bright, param->light_len);
	light_direction = vec_normalize(vec_minus(light.pos, info->hit_point));
	param->kd = fmax(vec_dotprod(info->norm_vec, light_direction), 0);
	param->diffuse = apply_bright(light.color, param->kd * bright);
	l_dot_n = vec_dotprod(light_direction, info->norm_vec);
	reflect_direction = \
		vec_minus(vec_scale(info->norm_vec, 2.0 * l_dot_n), light_direction);
	r_dot_v_alpha = pow(vec_dotprod(reflect_direction, v), param->alpha);
	param->specular = \
		add_color(param->specular, \
			apply_bright(light.color, param->ks * r_dot_v_alpha * bright));
	param->point_color = \
		add_color(param->point_color, param->diffuse);
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
