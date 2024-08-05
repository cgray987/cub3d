/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:43:19 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 13:38:46 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**dup_array(char **array, int height)
{
	char	**tmp;
	int		i;

	i = 0;
	tmp = malloc(sizeof(char *) * (height + 1));
	if (!tmp)
		return (NULL);
	while (i < height)
	{
		tmp[i] = ft_strdup(array[i]);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

int	get_map_height(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

int	get_map_width(char **map)
{
	int	i;
	int	j;
	int	j_max;

	i = 0;
	j = 0;
	j_max = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			j++;
			if (j > j_max)
				j_max = j;
		}
		i++;
	}
	return (j_max);
}

bool	init_map_file(t_map_file *map_file)
{
	map_file->map_height = get_map_height(map_file->map);
	map_file->map_width = get_map_width(map_file->map);
	if (map_file->map_width == 0)
	{
		dprintf(2, "Map size invalid!\n");
		return (false);
	}
	map_file->init_player_x = 0;
	map_file->init_player_y = 0;
	return (true);
}

void	free_map_file(t_map_file *map_file)
{
	free_array((void **)map_file->map);
	free(map_file);
}

/* // used for testing
void	print_fill_map(t_map_file *map_file, char **fill_map)
{
	int	i;
	int	j;
	i = 0;
	j = 0;
	printf("fill map\n");
	while (i < map_file->map_height)
	{
		while (j < map_file->map_width)
		{
			if (fill_map[i][j] == '1')
				printf("X");
			else if (ft_strchr("\t\n", fill_map[i][j]))
				printf(" ");
			else if (ft_strchr("NSWE", fill_map[i][j]))
				printf("P");
			else
				printf(" ");
			j++;
		}
		printf("\n");
		j = 0;
		i++;
	}
}
 */
