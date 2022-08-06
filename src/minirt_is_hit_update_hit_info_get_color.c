/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_is_hit_update_hit_info_get_color.c          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 23:52:03 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 18:35:04 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt_hit.h"

static t_color	get_color_tx(t_obj_info *obj, t_hit_info *info);
static void		update_n_vec_from_bm(t_obj_info *obj, t_hit_info *info);
static t_vec	get_n_vec_bm(t_obj_info *obj, t_hit_info *info, int x, int y);

t_color	get_point_color(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	int		flag;
	double	scale;

	get_uv_mapping(obj, info, &info->uv_map);
	if (obj->surface & CHECKER_BOARD)
	{
		scale = obj->map_scale * M_PI / 4.0;
		flag = (int)(info->uv_map.u / scale) + (int)(info->uv_map.v / scale);
		if (flag % 2)
			return (set_color(0, 0, 0));
		return (set_color(1, 1, 1));
	}
	else
	{
		if (obj->surface & BUMP_MAP)
			update_n_vec_from_bm(obj, info);
		if (obj->surface & TEXTURE)
			color = get_color_tx(obj, info);
		else
			color = obj->color;
	}
	return (color);
}

static t_vec	get_n_vec_bm(t_obj_info *obj, t_hit_info *info, int x, int y)
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

	x = ((int)(info->uv_map.u * BM_TX_SCALE)) % obj->bm.w;
	y = ((int)(info->uv_map.v * BM_TX_SCALE)) % obj->bm.h;
	if (x < 0)
		x += obj->bm.w;
	if (y < 0)
		y += obj->bm.h;
	info->norm_vec = get_n_vec_bm(obj, info, x, y);
}

static t_color	get_color_tx(t_obj_info *obj, t_hit_info *info)
{
	t_color	color;
	int		x;
	int		y;

	x = ((int)(info->uv_map.u * BM_TX_SCALE)) % obj->tx.w;
	y = ((int)(info->uv_map.v * BM_TX_SCALE)) % obj->tx.h;
	if (x < 0)
		x += obj->tx.w;
	if (y < 0)
		y += obj->tx.h;
	color = set_color_from_int(get_mlx_pixel_color(&obj->tx, x, y));
	return (color);
}

t_vec	get_normal_vector(t_vec dir, t_vec point, t_obj_info *obj)
{
	t_vec	norm;
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
		norm = vec_normalize(vec_crossprod(vec_minus(vec_scale(obj->orient, \
						obj->height), diff), vec_crossprod(diff, obj->orient)));
	}
	else
		norm = obj->orient;
	if (vec_dotprod(norm, dir) > 0)
		return (vec_scale(norm, (double)-1));
	return (norm);
}
