/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:38:13 by cgray             #+#    #+#             */
/*   Updated: 2024/07/08 17:58:53 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_array(void **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
	if (array)
		free(array);
}

void	free_cub_file(t_cub_file *cub_file)
{
	free(cub_file->NO);
	free(cub_file->SO);
	free(cub_file->WE);
	free(cub_file->EA);
	free(cub_file->floor);
	free(cub_file->ceiling);
	free_array((void **)cub_file->map);
	free(cub_file);
}

void	init_cub_file(t_cub_file *cub_file)
{
	cub_file->textures_done = false;
	cub_file->NO = NULL;
	cub_file->SO = NULL;
	cub_file->WE = NULL;
	cub_file->EA = NULL;
	cub_file->floor = NULL;
	cub_file->ceiling = NULL;
}

/* called in handle textures
	checks that line starts with dir (NO, SO, EA, WE, F, C)
	skips spaces in line, dups into cub struct
	returns 1 if successful*/
int		set_ceiling_floor(char *line, char *dir, t_cub_file *cub)
{
	int	size;

	size = 0;
	// printf("line: %s\n", line);
	if (line + 1 && !ft_strncmp(line, dir, 1))
	{
		line += 1;
		while (ft_isspace(*line))
			line++;
		while (*line++ != '\n' || ft_isspace(*line))
			size++;
		line -= size + 1;
		if (!ft_strncmp(dir, "C", 1))
			cub->ceiling = ft_strndup(line, size); //create function to extract color code rather than string
		else if (!ft_strncmp(dir, "F", 1))
			cub->floor = ft_strndup(line, size); //create function to extract color code rather than string
		return (1);
	}
	return (0);
}

/* called in handle textures
	checks that line starts with dir (NO, SO, EA, WE, F, C)
	skips spaces in line, dups into cub struct
	returns 1 if successful*/
int		set_texture_dir(char *line, char *dir, t_cub_file *cub)
{
	int	size;

	size = 0;
	// printf("line: %s\n", line);
	if (line + 2 && !ft_strncmp(line, dir, 2))
	{
		line += 2;
		while (ft_isspace(*line))
			line++;
		while (*line++ != '\n' || ft_isspace(*line))
			size++;
		line -= size + 1;
		if (!ft_strncmp(dir, "NO", 2))
			cub->NO = ft_strndup(line, size);
		else if (!ft_strncmp(dir, "SO", 2))
			cub->SO = ft_strndup(line, size);
		else if (!ft_strncmp(dir, "WE", 2))
			cub->WE = ft_strndup(line, size);
		else if (!ft_strncmp(dir, "EA", 2))
			cub->EA = ft_strndup(line, size);
		return (1);
	}
	return (0);
}
