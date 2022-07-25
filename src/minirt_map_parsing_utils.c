/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:58:51 by seseo             #+#    #+#             */
/*   Updated: 2022/07/25 15:35:21 by seseo            ###   ########.fr       */
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

int	args_len_check(char **args, int len)
{
	int	i;

	if (args == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (args[i])
		i++;
	if (len == i)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
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
	int	i;

	i = 0;
	while (i < 3)
	{
		if (orient[i] < -1 || orient[i] > 1)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
