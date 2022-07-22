/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils_lst.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:20:45 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 22:15:45 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_objadd_back(t_obj **lst, t_obj *new)
{
	t_obj	*tmp;

	tmp = *lst;
	if (lst && new)
	{
		if (*lst)
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*lst = new;
	}
}

void	ft_camadd_back(t_camera **lst, t_camera *new)
{
	t_camera	*tmp;

	tmp = *lst;
	if (lst && new)
	{
		if (*lst)
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*lst = new;
	}
}

void	ft_lightadd_back(t_light **lst, t_light *new)
{
	t_light	*tmp;

	tmp = *lst;
	if (lst && new)
	{
		if (*lst)
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*lst = new;
	}
}
