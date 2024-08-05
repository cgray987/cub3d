/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:18:24 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 13:57:46 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* called in key hook
	add angle and set 0 < direction < 2PI */
void	rotate_player(t_player *player, double angle)
{
	double	old_dir_x;
	double	old_dir_y;
	double	old_plane_x;
	double	old_plane_y;

	angle *= ROT_SPEED;
	old_dir_x = player->d->dx;
	old_dir_y = player->d->dy;
	old_plane_x = player->plane->dx;
	old_plane_y = player->plane->dy;
	player->dir += angle;
	if (player->dir == 0)
		player->dir += 2 * PI;
	if (player->dir > 2 * PI)
		player->dir -= 2 * PI;
	player->d->dx = player->d->dx * cos(angle) - player->d->dy * sin(angle);
	player->d->dy = old_dir_x * sin(angle) + old_dir_y * cos(angle);
	player->plane->dx = player->plane->dx * cos(angle)
		- player->plane->dy * sin(angle);
	player->plane->dy = old_plane_x * sin(angle) + old_plane_y * cos(angle);
}

/*	called in key hook
	*/
void	mouse_rotation(t_game *game)
{
	int32_t	mouse_x;
	int32_t	mouse_y;

	mouse_x = 0;
	mouse_y = 0;
	mlx_get_mouse_pos(game->mlx, &mouse_x, &mouse_y);
	rotate_player(game->player, (mouse_x - WIDTH / 2) * 0.01);
	mlx_set_mouse_pos(game->mlx, WIDTH / 2, HEIGHT / 2);
}

/* called in key hook
	move left,right,up,down in player direction */
void	move_player(t_player *player, int key, char **map)
{
	double	dx;
	double	dy;

	dx = player->d->dx;
	dy = player->d->dy;
	if (key == MLX_KEY_A)
		check_collision(player, player->x + (MOVE_SPEED / 2) * dy,
			player->y - (MOVE_SPEED / 2) * dx, map);
	if (key == MLX_KEY_D)
		check_collision(player, player->x - (MOVE_SPEED / 2) * dy,
			player->y + (MOVE_SPEED / 2) * dx, map);
	if (key == MLX_KEY_W)
		check_collision(player, player->x + MOVE_SPEED * dx,
			player->y + MOVE_SPEED * dy, map);
	if (key == MLX_KEY_S)
		check_collision(player, player->x - MOVE_SPEED * dx,
			player->y - MOVE_SPEED * dy, map);
}

/* this is such a broken way to do this */
void	open_door(mlx_key_data_t key, void *v_game)
{
	t_game	*game;
	int		i;
	int		j;
	int		margin;

	game = (t_game *)v_game;
	if (game->door_dist > 2)
		return ;
	margin = 2;
	i = (int)game->player->y - margin;
	if (key.key == MLX_KEY_SPACE && key.action == MLX_PRESS)
	{
		while (i < (int)game->player->y + margin
			&& i < game->map->map_height && i > 0)
		{
			j = (int)game->player->x - margin;
			while (j < (int)game->player->x + margin
				&& j < game->map->map_width && j > 0)
				door_handler(game, i, j++);
			i++;
		}
	}
}
