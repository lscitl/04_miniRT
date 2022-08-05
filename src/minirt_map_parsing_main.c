/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_map_parsing_main.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seseo <seseo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 15:17:21 by seseo             #+#    #+#             */
/*   Updated: 2022/08/06 00:50:22 by seseo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static char	**get_map_args(char *file_path);
static void	get_map_data_to_buffer(t_buffer *buf, int fd);
static int	check_invalid_map(t_map *map);

void	map_parsing(char *file_path, t_map *map)
{
	char		**map_args;

	ft_memset(map, 0, sizeof(t_map));
	map_args = get_map_args(file_path);
	if (set_map_data(map, map_args) == EXIT_FAILURE || \
		check_invalid_map(map) == EXIT_FAILURE)
	{
		ft_putendl_fd("minirt: Map Error", 2);
		exit(EXIT_FAILURE);
	}
	free_strs(map_args);
}

static char	**get_map_args(char *file_path)
{
	t_buffer	*buf;
	char		**map_args;
	char		*args;
	int			fd;

	if (ft_strncmp(file_path + ft_strlen(file_path) - 3, ".rt", -1))
	{
		printf("please input .rt file\n");
		exit(EXIT_FAILURE);
	}
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
	if (!map->light || !map->camera || !map->obj)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
