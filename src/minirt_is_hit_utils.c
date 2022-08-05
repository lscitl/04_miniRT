/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:03:47 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/05 13:28:43 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj);
static int		is_within_obj(t_vec intersect, t_obj_info *obj);
static t_color	get_point_color(t_obj_info *obj, t_hit_info *info);
static void		get_uv_mapping(t_obj_info *obj, t_hit_info *info, t_uv_map *uv_map);

int	update_hit_info(t_hit_info *info, t_obj_info *obj, double root)
{
	t_vec	hit_point;

	hit_point = vec_ray_at_distance(info->ray, root);
	if (obj->type == CYLINDER || obj->type == CONE)
	{
		if (!is_within_obj(hit_point, obj))
			return (-1);
	}
	info->hit_point = hit_point;
	info->distance = root;
	if (info->shadow_flag == TRUE)
		return (0);
	info->phong.kd = obj->kd;
	info->phong.ks = obj->ks;
	info->phong.alpha = obj->alpha;
	info->norm_vec = get_normal_vector(info->ray.direction, hit_point, obj);
	if (obj->surface)
		info->color = get_point_color(obj, info);
	else
		info->color = obj->color;
	return (0);
}

static t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj)
{
	t_vec	norm;
	t_vec	cone_top;
	t_vec	diff;

	diff = vec_minus(point, obj->pos);
	if (obj->type == SPHERE)
		norm = vec_normalize(diff);
	else if (obj->type == CYLINDER)
	{
		norm = vec_normalize(vec_crossprod(obj->orient, \
					vec_crossprod(diff, obj->orient)));
	}
	else if (obj->type == CONE)
	{
		cone_top = vec_plus(obj->pos, vec_scale(obj->orient, obj->height));
		norm = vec_normalize(vec_crossprod(vec_minus(cone_top, point), \
					vec_crossprod(diff, obj->orient)));
	}
	else
		norm = obj->orient;
	if (vec_dotprod(norm, dir) > 0)
		return (vec_scale(norm, (double)-1));
	return (norm);
}

static int	is_within_obj(t_vec intersect, t_obj_info *obj)
{
	double	projection;

	projection = vec_dotprod(vec_minus(intersect, obj->pos), obj->orient);
	return (projection >= -EPSILON && projection <= obj->height);
}

t_vec	get_n_vec_from_bm(t_obj_info *obj, t_hit_info *info, int x, int y)
{
	t_vec			ret;
	t_vec			bm_norm;
	t_vec			t;
	t_vec			b;
	unsigned int	pixel_val;

	b = info->uv_map.u_vec;
	t = info->uv_map.v_vec;
	pixel_val = get_mlx_pixel_color(&obj->bm, x, y);
	bm_norm.x = (pixel_val >> 16) & 0xFF;
	bm_norm.y = (pixel_val >> 8) & 0xFF;
	bm_norm.z = pixel_val & 0xFF;
	bm_norm = vec_scale(bm_norm, 2.0 / 255);
	bm_norm.x -= 1;
	bm_norm.y -= 1;
	bm_norm.z -= 1;
	ret.x = t.x * bm_norm.x + b.x * bm_norm.y + info->norm_vec.x * bm_norm.z;
	ret.y = t.y * bm_norm.x + b.y * bm_norm.y + info->norm_vec.y * bm_norm.z;
	ret.z = t.z * bm_norm.x + b.z * bm_norm.y + info->norm_vec.z * bm_norm.z;
	return (ret);
}

void	update_n_vec_from_bm(t_obj_info *obj, t_hit_info *info)
{
	int		x;
	int		y;
	double	scale;

	scale = obj->map_scale;
	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		x = ((int)floor(info->uv_map.u * scale)) % obj->bm.w;
		y = ((int)floor(info->uv_map.v * scale)) % obj->bm.h;
		if (x < 0)
			x += 200;
		if (y < 0)
			y += 200;
	}
	else
	{
		x = ((int)floor(info->uv_map.u * (obj->bm.w) * scale)) % obj->bm.w;
		y = ((int)floor(info->uv_map.v * (obj->bm.h) * scale)) % obj->bm.h;
	}
	info->norm_vec = get_n_vec_from_bm(obj, info, x, y);
}

t_color	get_color_from_tx(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	int		x;
	int		y;

	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		x = ((int)floor(info->uv_map.u * obj->map_scale)) % obj->tx.w;
		y = ((int)floor(info->uv_map.v * obj->map_scale)) % obj->tx.h;
		if (x < 0)
			x += 200;
		if (y < 0)
			y += 200;
	}
	else
	{
		x = ((int)floor(info->uv_map.u * (obj->tx.w) * 2)) % obj->tx.w;
		y = ((int)floor(info->uv_map.v * (obj->tx.h) * 2)) % obj->tx.h;
	}
	color = set_color_from_int(get_mlx_pixel_color(&obj->tx, x, y));
	return (color);
}

static t_color	get_color_from_obj(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	double	scale;
	int		flag;

	if (obj->surface & CHECKER_BOARD)
	{
		scale = obj->map_scale;
		flag = (floor(info->uv_map.u / scale) + floor(info->uv_map.v / scale));
		if (flag % 2)
			return (set_color(0, 0, 0));
		else
			return (set_color(1, 1, 1));
	}
	else
	{
		if (obj->surface & BUMP_MAP)
			update_n_vec_from_bm(obj, info);
		if (obj->surface & TEXTURE)
			color = get_color_from_tx(obj, info);
		else
			color = obj->color;
	}
	return (color);
}

static t_color	get_point_color(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;

	get_uv_mapping(obj, info, &info->uv_map);
	color = get_color_from_obj(obj, info);
	/* color = add_color(apply_bright(set_color(0.8, 0.8, 0.1), uv[0]), \ */
	/*         apply_bright(set_color(0.1, 0.4, 0.9), uv[1]));            */
	return (color);
}

// U, V in range [0, 1]
// plane -> UV coordinate
// sphere -> UV mapping
// cylinder -> theta, h
// cone -> r (distance from top_point), theta on the
static void	get_uv_mapping(t_obj_info *obj, t_hit_info *info, t_uv_map *uv_map)
{
	t_vec	d;

	find_uv_unit_vec(obj->orient, &info->uv_map.u_vec, &info->uv_map.v_vec);
	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		uv_map->u = vec_dotprod(vec_minus(info->hit_point, obj->pos), uv_map->u_vec);
		uv_map->v = vec_dotprod(vec_minus(info->hit_point, obj->pos), uv_map->v_vec);
	}
	else if (obj->type == SPHERE)
	{
		d = vec_normalize(vec_minus(obj->pos, info->hit_point));
		uv_map->u = 0.5 + atan2(d.x, d.z) / (2.0 * M_PI);
		uv_map->v = 0.5 + asin(d.y) / M_PI;
	}
	else
	{
		uv_map->u = acos(vec_dotprod(vec_normalize(vec_minus(vec_minus(info->hit_point, obj->pos), vec_scale(obj->orient, vec_dotprod((vec_minus(info->hit_point, obj->pos)), obj->orient)))), uv_map->u_vec)) / M_PI;
		uv_map->v = vec_dotprod(obj->orient, vec_minus(info->hit_point, obj->pos)) / obj->height;
		if (obj->type == CONE)
			uv_map->v = 1.0 - uv_map->v;
	}
}
