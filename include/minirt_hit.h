/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_hit.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/25 21:02:31 by seseo             #+#    #+#             */
/*   Updated: 2022/08/02 21:05:10 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_HIT_H
# define MINIRT_HIT_H

# include "minirt.h"

// vec_dotprod(normal_vector, ray.direction);
// (orig + t * dir) hit object
typedef struct s_hit_info
{
	t_vec	hit_point;
	t_color	color;
	t_vec	norm_vec;
	double	distance;
	int		direction;
}	t_hit_info;

int		is_hit(t_ray ray, t_obj_info *obj, t_hit_info *info);

int		update_hit_info(t_vec dir, t_hit_info *info, t_obj_info *obj, t_vec hit_point, double root);
int		is_within_obj(t_vec intersect, t_obj_info *obj);
void	calc_coeff(double coeff[3], t_ray ray, t_obj_info *obj);

// minirt_phong_and_shadow.c
t_color	phong_reflection(t_map_info *map, t_hit_info *info, t_vec v);

#endif
