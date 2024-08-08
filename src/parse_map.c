/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 12:23:13 by cgray             #+#    #+#             */
/*   Updated: 2024/08/08 16:43:28 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* loops through map, looking for NSWE. returns false if no or multi player
sets init player pos in map_file */
static bool	check_player_direction(t_map_file *map_file)
{
	int	i;
	int	j;
	int	multi_player;

	multi_player = 0;
	i = 0;
	while (map_file->map[i])
	{
		j = 0;
		while (map_file->map[i][j])
		{
			if (ft_strchr("NSWE", map_file->map[i][j]))
			{
				multi_player++;
				map_file->init_direction = map_file->map[i][j];
				map_file->init_player_x = j;
				map_file->init_player_y = i;
			}
			j++;
		}
		i++;
	}
	if (multi_player != 1)
		return (dprintf(2, "Invalid player!\n"), false);
	return (true);
}

/* starts at a pixel inside the map. checks that initial place
and up, down, left and right are filled. called for every '0' and
initial player position*/
static int	boundary_fill(int i, int j, char **map)
{
	if ((i == 0 || j == 0) && (map[i][j] == '0' || map[i][j] == 'D'))
		return (0);
	else if (map[i - 1][j] == ' ' || map[i + 1][j] == ' ' \
		|| map[i][j - 1] == ' ' || map[i][j + 1] == ' ' \
		|| !map[i - 1][j] || !map[i + 1][j] \
		|| !map[i][j - 1] || !map[i][j + 1])
		return (0);
	else if (ft_strchr("NSWE", map[i][j])
		&& (map[i - 1][j] == '1' && map[i + 1][j] == '1' \
		&& map[i][j - 1] == '1' && map[i][j + 1] == '1'))
		return (0);
	return (1);
}

/* loops through entire map, checking if '0's and player pos are enclosed */
static bool	boundary_fill_loop(int p_x, int p_y, char **fill_map)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (fill_map[i])
	{
		j = 0;
		while (fill_map[i][j])
		{
			if (fill_map[i][j] == 'D' && !boundary_fill(i, j, fill_map))
				return (false);
			if (fill_map[i][j] == '0' && !boundary_fill(i, j, fill_map))
				return (false);
			if (i == p_y && j == p_x && !boundary_fill(i, j, fill_map))
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

/* dups map into 'fill_map' in order to test if walls are enclosed
returns false if walls are not */
static bool	check_walls(t_map_file *map_file)
{
	char	**fill_map;
	bool	surrounded;

	surrounded = true;
	fill_map = dup_array(map_file->map, map_file->map_height);
	if (!boundary_fill_loop(map_file->init_player_x,
			map_file->init_player_y, fill_map))
	{
		dprintf(2, "Map not surrounded by walls "
			"or player is enclosed!\n");
		surrounded = false;
	}
	free_array((void **)fill_map);
	return (surrounded);
}

/* called in parse_data
	initialize t_map_file, check player direction
	check walls are enclosed */
bool	parse_map(t_cub_file *cub_file, t_game *game)
{
	bool	check;

	game->map = malloc(sizeof(t_map_file));
	game->map->map = dup_array(cub_file->map, get_map_height(cub_file->map));
	check = true;
	if (init_map_file(game->map) == false)
		check = false;
	if (check_player_direction(game->map) == false)
		check = false;
	if (check == true && check_walls(game->map) == false)
		check = false;
	init_player(game);
	get_init_direction(game);
	game->ceiling = get_ceiling_floor_color(cub_file->ceiling);
	game->floor = get_ceiling_floor_color(cub_file->floor);
	if (game->ceiling == -42 || game->floor == -42)
		check = false;
	if (check == true
		&& load_textures(game, cub_file) == false)
		check = false;
	free_cub_file(cub_file);
	return (check);
}
