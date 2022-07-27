/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_utils_lst.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 16:20:45 by seseo             #+#    #+#             */
/*   Updated: 2022/07/27 15:12:17 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	lst_cnt(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int	lst_cnt_obj(t_obj *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		if (lst->type == CYLINDER)
			i += 2;
		else if (lst->type == CONE)
			i += 1;
		i++;
		lst = lst->next;
	}
	return (i);
}

void	ft_lst_node_clear(t_list *lst)
{
	t_list	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
}
