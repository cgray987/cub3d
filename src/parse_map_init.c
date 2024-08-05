/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 13:07:19 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 16:23:43 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* sets plane and player direction from map
	other direction component set to 0 in
	init player */
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
}

void	init_player(t_game *game)
{
	int	h;
	int	w;

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
	w = WIDTH / game->map->map_width / 3;
	h = HEIGHT / game->map->map_height / 3;
	game->mini_scale = (w + h) / 2;
}

/* o_tmp is so I can do pointer math on tmp and
still free it later */
int	get_ceiling_floor_color(char *color)
{
	int		r;
	int		g;
	int		b;
	char	*tmp;
	char	*o_tmp;

	o_tmp = ft_strdup(color);
	tmp = o_tmp;
	r = ft_atoi(color);
	g = ft_atoi(ft_strchr(color, ',') + 1);
	tmp = ft_strchr(color, ',') + 1;
	b = ft_atoi(ft_strchr(tmp, ',') + 1);
	free(o_tmp);
	if (r < 0 || r > 255
		|| g < 0 || g > 255
		|| b < 0 || b > 255)
	{
		dprintf(2, "Floor/Ceiling colors must be in range 0 - 255!\n");
		return -1;
	}
	return (combine_color(r, g, b, 0xff));
}

/* loads png texture, returning false if any fail */
bool	load_texture_error(t_game *game, t_cub_file *cub_file)
{
	int	i;

	i = 0;
	game->texture[0] = mlx_load_png(cub_file->so);
	game->texture[1] = mlx_load_png(cub_file->no);
	game->texture[2] = mlx_load_png(cub_file->ea);
	game->texture[3] = mlx_load_png(cub_file->we);
	while (i++ < 3)
	{
		if (!game->texture[i])
			return (false);
	}
	return (true);
}

/* loads texture and checks sizes */
bool	load_textures(t_game *game, t_cub_file *cub_file)
{
	int	i;

	i = 0;
	if (!load_texture_error(game, cub_file))
	{
		dprintf(2, "Textures failed to load!\n");
		while (i++ < 3)
		{
			if (game->texture[i])
				mlx_delete_texture(game->texture[i]);
		}
		return (false);
	}
	game->map->texture_size = game->texture[0]->height;
	i = 0;
	while (i++ < 3)
	{
		if ((int)game->texture[i]->height != game->map->texture_size
			|| (int)game->texture[i]->width != game->map->texture_size)
		{
			dprintf(2, "Texture %d has unexpected size!\n", i);
			return (false);
		}
	}
	return (true);
}
