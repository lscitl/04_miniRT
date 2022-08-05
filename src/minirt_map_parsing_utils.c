/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:58:51 by seseo             #+#    #+#             */
/*   Updated: 2022/08/05 20:55:09 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	replace_all_white_space_to_sp(char *str)
{
	while (*str)
	{
		if (9 <= *str && *str <= 13)
			*str = ' ';
		str++;
	}
}

int	get_args_len(char **args)
{
	int	i;

	if (args == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	is_all_digit(char *str)
{
	if (str == NULL)
		return (FALSE);
	while (*str)
	{
		if (!ft_isdigit(*str++))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	orient_check(double orient[3])
{
	double	len;

	if (fabs(orient[0]) > 1 || fabs(orient[1]) > 1 || fabs(orient[2]) > 1)
		return (EXIT_FAILURE);
	len = orient[0] * orient[0] + orient[1] * orient[1] + orient[2] * orient[2];
	len = sqrt(len);
	if (fabs(len) < EPSILON)
		return (EXIT_FAILURE);
	orient[0] = orient[0] / len;
	orient[1] = orient[1] / len;
	orient[2] = orient[2] / len;
	return (EXIT_SUCCESS);
}
