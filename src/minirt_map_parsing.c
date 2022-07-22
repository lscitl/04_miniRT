/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 15:17:21 by seseo             #+#    #+#             */
/*   Updated: 2022/07/21 22:47:35 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	**get_map_args(char *file_path);
static void	get_map_data_to_buffer(t_buffer *buf, int fd);

t_map	*map_parsing(char *file_path)
{
	t_map		*map;
	char		**map_args;

	map = ft_malloc(sizeof(t_map));
	ft_memset(map, 0, sizeof(t_map));
	map_args = get_map_args(file_path);
	if (set_map_data(map, map_args) || check_invalid_map(map))
	{
		ft_putendl_fd("minirt: Map Error", 2);
		exit(EXIT_FAILURE);
	}
	return (map);
}

static char	**get_map_args(char *file_path)
{
	t_buffer	*buf;
	char		**map_args;
	char		*args;
	int			fd;

	fd = open(file_path, O_RDONLY);
	if (fd == -1)
	{
		perror("miniRT");
		exit(EXIT_FAILURE);
	}
	buf = create_buf();
	get_map_data_to_buffer(buf, fd);
	close(fd);
	args = put_str(buf);
	map_args = ft_split(args, '\n');
	free(args);
	del_buf(buf);
	return (map_args);
}

static void	get_map_data_to_buffer(t_buffer *buf, int fd)
{
	char		*line;

	while (TRUE)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		add_str(buf, line);
		free(line);
	}
}

static int	check_invalid_map(t_map *map)
{
	if (!map->ambi_light || !map->light || !map->camera || !map->obj)
		return (EXIT_FAILURE);
}
