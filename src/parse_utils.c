/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:38:13 by cgray             #+#    #+#             */
/*   Updated: 2024/08/06 13:31:11 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_cub_file(t_cub_file *cub_file)
{
	cub_file->textures_done = false;
	cub_file->no = NULL;
	cub_file->so = NULL;
	cub_file->we = NULL;
	cub_file->ea = NULL;
	cub_file->floor = NULL;
	cub_file->ceiling = NULL;
}

/* called in handle textures
	checks that line starts with dir (NO, SO, EA, WE, F, C)
	skips spaces in line, dups into cub struct
	returns 1 if successful*/
int	set_ceiling_floor(char *line, char *dir, t_cub_file *cub)
{
	int	size;

	size = 0;
	if (line + 1 && !ft_strncmp(line, dir, 1))
	{
		line += 1;
		while (ft_isspace(*line))
			line++;
		while (*line++ != '\n' || ft_isspace(*line))
			size++;
		line -= size + 1;
		if (!ft_strncmp(dir, "C", 1))
			cub->ceiling = ft_strndup(line, size);
		else if (!ft_strncmp(dir, "F", 1))
			cub->floor = ft_strndup(line, size);
		return (1);
	}
	return (0);
}

static void	copy_into_struct(char *line, char *dir, int size, t_cub_file *cub)
{
	if (!ft_strncmp(dir, "NO", 2))
		cub->no = ft_strndup(line, size);
	else if (!ft_strncmp(dir, "SO", 2))
		cub->so = ft_strndup(line, size);
	else if (!ft_strncmp(dir, "WE", 2))
		cub->we = ft_strndup(line, size);
	else if (!ft_strncmp(dir, "EA", 2))
		cub->ea = ft_strndup(line, size);
}

/* called in handle textures
	checks that line starts with dir (NO, SO, EA, WE, F, C)
	skips spaces in front and behind path textures, dups into cub struct
	returns 1 if successful*/
int	set_texture_dir(char *line, char *dir, t_cub_file *cub)
{
	int	size;

	size = 0;
	if (line + 2 && !ft_strncmp(line, dir, 2))
	{
		line += 2;
		while (ft_isspace(*line))
			line++;
		while (*line++ != '\n')
		{
			size++;
			if (ft_isspace(*line))
				break ;
		}
		line -= size;
		copy_into_struct(line, dir, size, cub);
		return (1);
	}
	return (0);
}
