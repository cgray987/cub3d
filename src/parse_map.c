/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 12:23:13 by cgray             #+#    #+#             */
/*   Updated: 2024/06/25 17:18:39 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_map_height(t_map_file *map_file)
{
	int	i;

	i = 0;
	while (map_file->map[i])
		i++;
	return (i);
}

static int	get_map_width(t_map_file *map_file)
{
	int	i;
	int	j;
	int	j_max;

	i = 0;
	j = 0;
	j_max = 0;
	while (map_file->map[i])
	{
		j = 0;
		while (map_file->map[i][j])
		{
			j++;
			if (j > j_max)
				j_max = j;
		}
		i++;
	}
	return (j_max);
}

static bool	init_map_file(t_map_file *map_file, t_map_valid *valid)
{
	valid->valid_ceiling = false;
	valid->valid_direction = false;
	valid->valid_floor = false;
	valid->valid_ceiling = false;

	map_file->map_height = get_map_height(map_file);
	map_file->map_width = get_map_width(map_file);
	if (map_file->map_width == -1)
		return (false);
	map_file->init_player_x = 0;
	map_file->init_player_y = 0;
	// map_file->ceiling_color = {0, 0, 0};
	// map_file->floor_color = {0, 0, 0};
	return (true);
}

static bool	check_player_direction(t_map_file *map_file, t_map_valid *valid)
{
	int	i;
	int	j;
	int	multi_player;

	multi_player = 0;
	i = 0;
	j = 0;
	while (i < map_file->map_height)
	{
		while (j < map_file->map_width)
		{
			if (map_file->map[i][j] == 'N' || map_file->map[i][j] == 'S'
				|| map_file->map[i][j] == 'W' || map_file->map[i][j] == 'E')
			{
				multi_player++;
				// map_file->map[i][j] = '0'; //replace player with '0' as a valid space
				map_file->init_direction = map_file->map[i][j];
				map_file->init_player_x = j;
				map_file->init_player_y = i;
				valid->valid_direction = true;
			}
			j++;

		}
		j = 0;
		i++;
	}
	if (multi_player != 1)
		return (false);
	return (true);
}

void	free_array(void **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
}

static int	boundary_fill(int i, int j, char **fill_map)
{
	if (i < 0 || !fill_map[i]
		|| j < 0 || (fill_map[i] && !fill_map[i][j]))
		return (1);
	else if (fill_map[i][j] == '1')
		return (0);
	fill_map[i][j] = '1';
	// printf("changed fill map\n");
	return (
	boundary_fill(i - 1, j, fill_map)
	+ boundary_fill(i + 1, j, fill_map)
	+ boundary_fill(i, j - 1, fill_map)
	+ boundary_fill(i, j + 1, fill_map));
}

/* need to also check interior walls with holes */
static bool	check_walls(t_map_file *map_file)
{
	int	i;
	int	j;
	int	x;
	int	y;
	char	**fill_map;
	bool	surrounded;

	i = 0;
	fill_map = malloc(sizeof(char *) * map_file->map_height + 1);
	if (!fill_map)
		return (false);
	while (i < map_file->map_height)
	{
		fill_map[i] = ft_strdup(map_file->map[i]);
		i++;
	}
	fill_map[i] = NULL;
	x = map_file->init_player_x;
	y = map_file->init_player_y;
	i = 0;
	surrounded = true;
	int test = boundary_fill(y, x, fill_map);
	if (test > 0)
		surrounded = false;
	// while (fill_map[i])
	// {
	// 	j = 0;
	// 	while (fill_map[i][j])
	// 	{
	// 		if (fill_map[i][j] == '0' && boundary_fill(i, j, fill_map))
	// 		{
	// 			surrounded = false;
	// 			break ;
	// 		}
	// 		j++;
	// 	}
	// 	i++;
	// }

	i = 0;
	j = 0;
	printf("fill map\n");
	while (i < map_file->map_height)
	{
		while (j < map_file->map_width)
		{
			if (fill_map[i][j] == '1')
				printf("X");
			else
				printf(" ");
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
	free_array((void **)fill_map);
	return (surrounded);
}


bool	parse_map(t_cub_file *cub_file)
{
	t_map_file	*map_file;
	t_map_valid	*valid;
	map_file = malloc(sizeof(t_map_file));
	valid = malloc(sizeof(t_map_valid));
	map_file->map = cub_file->map;

	if (init_map_file(map_file, valid) == false)
	{
		dprintf(2, "Map size invalid!\n");
		return (false);
	}
	if (check_player_direction(map_file, valid) == false)
	{
		dprintf(2, "Invalid player!\n");
		return (false);
	}
	if (check_walls(map_file) == false)
	{
		dprintf(2, "Map not surrounded by walls!\n");
		return (false);
	}

	return (true);
}
