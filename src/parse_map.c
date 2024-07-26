/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 12:23:13 by cgray             #+#    #+#             */
/*   Updated: 2024/07/26 18:09:45 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* loops through map, looking for NSWE. returns false if no or multi player
sets init player pos in map_file */
static bool	check_player_direction(t_map_file *map_file) //, t_map_valid *valid
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
			if (map_file->map[i][j] == 'N' || map_file->map[i][j] == 'S'
				|| map_file->map[i][j] == 'W' || map_file->map[i][j] == 'E')
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
		return (false);
	return (true);
}

/* starts at a pixel inside the map. checks that initial place
and up, down, left and right are filled. called for every '0' and
initial player position*/
static int	boundary_fill(int i, int j, char **map)
{
	if ((i == 0 || j == 0) && map[i][j] == '0')
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

/* ft_strchr("NSWE", map[i - 1][j]) \
		|| ft_strchr("NSWE", map[i + 1][j]) \
		|| ft_strchr("NSWE", map[i][j - 1]) \
		|| ft_strchr("NSWE", map[i][j + 1]) \ */

/* loops through entire map, checking if '0's and player pos are enclosed */
static bool boundary_fill_loop(int p_x, int p_y, char **fill_map)
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

	fill_map = dup_array(map_file->map, map_file->map_height);
	surrounded = true;
	surrounded = boundary_fill_loop(map_file->init_player_x, map_file->init_player_y, fill_map);
	// print_fill_map(map_file, fill_map);
	free_array((void **)fill_map);
	return (surrounded);
}

void	get_init_direction(t_game *game)
{
	if (game->map->init_direction == 'N')
	{
		game->player->d->dy = -1;
		game->player->plane->dx = 0.66;
	}
	if (game->map->init_direction == 'S')
	{
		game->player->d->dy = 1;
		game->player->plane->dx = -0.66;
	}
	if (game->map->init_direction == 'W')
	{
		game->player->d->dx = -1;
		game->player->plane->dy = 0.66;
	}
	if (game->map->init_direction == 'E')
	{
		game->player->d->dx = 1;
		game->player->plane->dy = -0.66;
	}
	// game->player->d->dx = cos(game->player->dir);
	// game->player->d->dy = sin(game->player->dir);
}

void	init_player(t_game *game)
{
	game->player = malloc(sizeof(t_player));
	game->player->x = game->map->init_player_x + 0.5;
	game->player->y = game->map->init_player_y + 0.5;
	game->player->dir = 0;
	game->player->d = malloc(sizeof(t_direction));
	game->player->plane = malloc(sizeof(t_direction));
	game->player->d->dx = 0;
	game->player->d->dy = 0;
	game->player->plane->dx = 0;
	game->player->plane->dy = 0;
}

int	get_ceiling_floor_color(char *color)
{
	int	r;
	int	g;
	int	b;
	char	*tmp;
	char	*o_tmp;

	o_tmp = ft_strdup(color);
	tmp = o_tmp;
	r = ft_atoi(color);
	g = ft_atoi(ft_strchr(color, ',') + 1);
	tmp = ft_strchr(color, ',') + 1;
	b = ft_atoi(ft_strchr(tmp, ',') + 1);
	free(o_tmp);
	return (combine_color(r, g, b));
}

/* called in parse_data
	initialize t_map_file, check player direction
	check walls are enclosed */
bool	parse_map(t_cub_file *cub_file, t_game *game)
{
	bool	check;
	// t_map_file	*map_file;
	// t_map_valid	*valid;
	game->map = malloc(sizeof(t_map_file));
	// valid = malloc(sizeof(t_map_valid));
	game->map->map = dup_array(cub_file->map, get_map_height(cub_file->map));
	check = true;
	if (init_map_file(game->map) == false)
	{
		dprintf(2, "Map size invalid!\n");
		check = false;
	}
	if (check_player_direction(game->map) == false)
	{
		dprintf(2, "Invalid player!\n");
		check = false;
	}
	if (check_walls(game->map) == false)
	{
		dprintf(2, "Map not surrounded by walls "
			"or player is enclosed!\n");
		check = false;
	}
	init_player(game);
	get_init_direction(game);
	game->ceiling = get_ceiling_floor_color(cub_file->ceiling);
	game->floor = get_ceiling_floor_color(cub_file->floor);
	// game->map = map_file;
	free_cub_file(cub_file);

	// free_map_file(map_file);

	return (check);
}
