/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_math.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chanhpar <chanhpar@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:19:51 by chanhpar          #+#    #+#             */
/*   Updated: 2022/08/02 20:22:56 by chanhpar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_MATH_H
# define MINIRT_MATH_H

# include <math.h>

# define EPSILON			1e-6

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;

typedef struct s_ray
{
	t_vec	orig;
	t_vec	direction;
}	t_ray;

int		solve_quadratic(double coeff[3], double root[2]);
int		solve_linear(double coeff[3], double root[2]);

t_vec	vec_make(double value[3]);
double	vec_dotprod(t_vec vec1, t_vec vec2);
t_vec	vec_crossprod(t_vec vec1, t_vec vec2);
double	vec_cos(t_vec vec1, t_vec vec2);
double	vec_sin(t_vec vec1, t_vec vec2);
t_vec	vec_normalize(t_vec vec);
double	vec_length(t_vec vec);
t_vec	vec_plus(t_vec vec1, t_vec vec2);
t_vec	vec_minus(t_vec vec1, t_vec vec2);
t_vec	vec_scale(t_vec vec1, double scale);
t_vec	vec_ray_at_distance(t_ray ray, double distance);
int		is_zero(double value);

#endif
