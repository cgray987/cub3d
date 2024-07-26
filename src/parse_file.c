/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:43:25 by cgray             #+#    #+#             */
/*   Updated: 2024/07/08 17:53:05 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* called in check_argument, returns joined map string
	from line */
static char	*map_str(char *line, char *map)
{
	char	*new_map;
	if (*line == '\n')
		return (map);
	if (!map && line)
		new_map = ft_strdup(line);
	else
	{
		new_map = ft_strjoin(map, line);
		free(map);
	}

	return (new_map);
}

static void	handle_textures(char *line, t_cub_file *cub)
{
	while (ft_isspace(*line))
		line++;
	if (set_texture_dir(line, "NO", cub))
		return ;
	else if (set_texture_dir(line, "SO", cub))
		return ;
	else if (set_texture_dir(line, "WE", cub))
		return ;
	else if (set_texture_dir(line, "EA", cub))
		return ;
	else if (set_ceiling_floor(line, "C", cub))
		return ;
	else if (set_ceiling_floor(line, "F", cub))
		return ;
	if (cub->NO && cub->SO && cub->WE && cub->EA
		&& cub->ceiling && cub->floor)
		cub->textures_done = true;
}

/* helper function bc of norminette, note map is not a 2d array, its
	used as pointer to string map to init and use in check_argument */
static bool	map_reader(char *line, char **map, t_cub_file *cub_struct)
{
	if (!line)
		return (true);
	if (cub_struct->textures_done == false)
		handle_textures(line, cub_struct);
	if (cub_struct->textures_done == true)
		*map = map_str(line, *map);
	free(line);
	return (false);
}

/* reads each line of .cub, placing each line into appropriate
	place in cub struct.
	textures must come before map, so if textures don't finish
	returns false*/
static bool check_argument(char *arg, t_cub_file *cub_struct)
{
	char		*line;
	char		*map;
	int			fd;

	fd = open(arg, O_RDONLY, 0644);
	if (fd == -1)
		return (false);
	map = NULL; //because I check if map is null later
	while (1)
	{
		line = get_next_line(fd);
		if (map_reader(line, &map, cub_struct) == true)
			break ;
	}
	close(fd);
	if (map)
		cub_struct->map = ft_split(map, '\n');
	else
	{
		free(map);
		return (false);
	}
	free(map);
	return (true);
}

/* parses .cub map into cub struct.
	inits cub struct, displays error if .cub can't be opened
	or is wrong format */
int	parse_data(char *arg, t_game *game)
{
	t_cub_file	*cub_file;
	// game = malloc(sizeof(t_game));
	cub_file = malloc(sizeof(t_cub_file));
	init_cub_file(cub_file);
	if (check_argument(arg, cub_file) == false)
	{
		dprintf(2, "Invalid .cub file!\n");
		return (1);
	}
	int i = 0;
	while (cub_file->map[i])
		printf("%s\n", cub_file->map[i++]);
	if (parse_map(cub_file, game) == false)
	{
		dprintf(2, "Invalid map!\n");
		return (1);
	}

	// free_cub_file(cub_file);
	// free(game);

	return (0);
}
