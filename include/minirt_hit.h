/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_hit.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 21:02:31 by seseo             #+#    #+#             */
/*   Updated: 2022/08/05 02:05:19 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_HIT_H
# define MINIRT_HIT_H

# include "minirt.h"

# define LIGHT_LEN_LIMIT 4000

typedef struct s_uv_map
{
	double	u;
	double	v;
	t_vec	u_vec;
	t_vec	v_vec;
}	t_uv_map;

// vec_dotprod(normal_vector, ray.direction);
// (orig + t * dir) hit object
typedef struct s_hit_info
{
	t_phong		phong;
	t_vec		hit_point;
	t_color		color;
	t_vec		norm_vec;
	double		distance;
	int			direction;
	t_ray		ray;
	t_uv_map	uv_map;
	int			shadow_flag;
}	t_hit_info;

int		is_hit(t_obj_info *obj, t_hit_info *info);

int	update_hit_info(t_hit_info *info, t_obj_info *obj, double root);
void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj);

// minirt_phong_and_shadow.c
t_phong	phong_reflection(t_map_info *map, t_hit_info *info, t_vec v);

#endif
