/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:58:51 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 23:12:06 by seseo            ###   ########.fr       */
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

int	color_check(int color[3])
{
	int	i;

	i = 0;
	if (i < 3)
	{
		if (color[i] < 0 || color[i] > 255)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	orient_check(double orient[3])
{
	int	i;

	i = 0;
	if (i < 3)
	{
		if (orient[i] < 0 || orient[i] > 1)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

void	print_strs(char **strs)
{
	while (*strs)
		printf("%s\n", *strs++);
}
