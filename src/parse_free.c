/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:28:31 by cgray             #+#    #+#             */
/*   Updated: 2024/08/06 13:30:41 by cgray            ###   ########.fr       */
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
	free(cub_file->no);
	free(cub_file->so);
	free(cub_file->we);
	free(cub_file->ea);
	free(cub_file->floor);
	free(cub_file->ceiling);
	free_array((void **)cub_file->map);
	free(cub_file);
}

void	free_map_file(t_map_file *map_file)
{
	free_array((void **)map_file->map);
	free(map_file);
}
