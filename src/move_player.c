/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:18:24 by cgray             #+#    #+#             */
/*   Updated: 2024/07/04 16:21:03 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* checks if player will hit wall with new position */
void	check_walls(t_player *player, double new_x, double new_y, char **map)
{
	if (map[(int)new_y][(int)player->x] != '1')
		player->y = new_y;
	if (map[(int)player->y][(int)new_x] != '1')
		player->x = new_x;
}

/* called in key hook
	add angle and set 0 < direction < 2PI */
void	rotate_player(t_player *player, double angle)
{
	player->dir += angle;
	if (player->dir < 0)
		player->dir = 2 * PI;
	if (player->dir > 2 * PI)
		player->dir = 0;
}

/* called in key hook
	move left,right,up,down in player direction */
void	move_player(t_player *player, int key, char **map)
{
	double	dx;
	double	dy;

	dx = cos(player->dir);
	dy = sin(player->dir);
	if (key == MLX_KEY_A)
		check_walls(player, player->x - .05 * dy,
					player->y + .1 * dx, map);
	if (key == MLX_KEY_D)
		check_walls(player, player->x + .05 * dy,
					player->y - .1 * dx, map);
	if (key == MLX_KEY_W)
		check_walls(player, player->x + .1 * dx,
					player->y + .1 * dy, map);
	if (key == MLX_KEY_S)
		check_walls(player, player->x - .1 * dx,
					player->y - .1 * dy, map);
}
