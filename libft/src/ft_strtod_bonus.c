/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtod_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 14:32:39 by seseo             #+#    #+#             */
/*   Updated: 2022/07/19 16:31:50 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		skip_space_and_get_sign(char **nptr);
static int		ft_isspace(char c);
static double	get_int_part(char **nptr);

double	ft_strtod(char *nptr, char **endptr)
{
	double	ret;
	double	dec_p;
	int		sign;

	ret = 0.0;
	sign = 1;
	if (nptr != NULL)
	{
		sign = skip_space_and_get_sign(&nptr);
		ret = (double)get_int_part(&nptr);
		if (*nptr == '.')
		{
			nptr++;
			dec_p = 1.0;
			while (*nptr && ft_isdigit(*nptr))
			{
				dec_p /= 10;
				ret += (dec_p * (*nptr++ - '0'));
			}
		}
	}
	if (endptr)
		*endptr = nptr;
	return (ret * sign);
}

static int	skip_space_and_get_sign(char **nptr)
{
	int		sign;

	while (ft_isspace(**nptr))
		(*nptr)++;
	sign = 1;
	if ((**nptr) == '+' || (**nptr) == '-')
	{
		if (**nptr == '-')
			sign = -1;
		(*nptr)++;
	}
	return (sign);
}

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static double	get_int_part(char **nptr)
{
	char	*tmp;
	double	ret;

	ret = 0.0;
	tmp = *nptr;
	while (*tmp && ft_isdigit(*tmp))
	{
		ret *= 10;
		ret += (*tmp++ - '0');
	}
	*nptr = tmp;
	return (ret);
}
