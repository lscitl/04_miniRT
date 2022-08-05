/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info_get_color.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 23:52:03 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 01:15:15 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_color	get_color_from_tx(t_obj_info *obj, t_hit_info *info);
static void		update_n_vec_from_bm(t_obj_info *obj, t_hit_info *info);
static t_vec	get_n_vec_from_bm(t_obj_info *obj, t_hit_info *info, int x, int y);

t_color	get_point_color(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	double	scale;
	int		flag;

	get_uv_mapping(obj, info, &info->uv_map);
	if (obj->surface & CHECKER_BOARD)
	{
		scale = obj->map_scale;
		if (obj->type == PLANE || obj->type == CIRCLE)
		{
			flag = (floor(info->uv_map.u / scale) + floor(info->uv_map.v / scale));
		}
		else
		{
			flag = (floor(info->uv_map.u * 2 * M_PI * obj->radius / scale) + floor(info->uv_map.v * vec_dotprod(obj->orient, vec_minus(info->hit_point, obj->pos)) / scale));
		}
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

static t_vec	get_n_vec_from_bm(t_obj_info *obj, t_hit_info *info, int x, int y)
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
	return (vec_normalize(ret));
}

static void	update_n_vec_from_bm(t_obj_info *obj, t_hit_info *info)
{
	int		x;
	int		y;

	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		x = ((int)floor(info->uv_map.u * PLANE_TX_SCALE)) % obj->bm.w;
		y = ((int)floor(info->uv_map.v * PLANE_TX_SCALE)) % obj->bm.h;
		if (x < 0)
			x += obj->bm.w;
		if (y < 0)
			y += obj->bm.h;
	}
	else
	{
		x = ((int)floor(info->uv_map.u * (obj->bm.w))) % obj->bm.w;
		y = ((int)floor(info->uv_map.v * (obj->bm.h))) % obj->bm.h;
	}
	info->norm_vec = get_n_vec_from_bm(obj, info, x, y);
}

static t_color	get_color_from_tx(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	int		x;
	int		y;

	if (obj->type == PLANE || obj->type == CIRCLE)
	{
		x = ((int)floor(info->uv_map.u * PLANE_TX_SCALE)) % obj->tx.w;
		y = ((int)floor(info->uv_map.v * PLANE_TX_SCALE)) % obj->tx.h;
		if (x < 0)
			x += obj->tx.w;
		if (y < 0)
			y += obj->tx.h;
	}
	else
	{
		x = ((int)floor(info->uv_map.u * (obj->tx.w))) % obj->tx.w;
		y = ((int)floor(info->uv_map.v * (obj->tx.h))) % obj->tx.h;
	}
	color = set_color_from_int(get_mlx_pixel_color(&obj->tx, x, y));
	return (color);
}
