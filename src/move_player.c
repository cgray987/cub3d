/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 20:18:24 by cgray             #+#    #+#             */
/*   Updated: 2024/07/24 17:15:33 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* checks if player will hit wall with new position */
void	check_collision(t_player *player, double new_x, double new_y,
		char **map)
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
	double oldDirX, oldDirY;
	double oldPlaneX, oldPlaneY;
	double rotSpeed = 0.23;

	// Apply rotation speed to angle
	angle *= rotSpeed;

	// Store old directions before modifying
	oldDirX = player->d->dx;
	oldDirY = player->d->dy;
	oldPlaneX = player->plane->dx;
	oldPlaneY = player->plane->dy;

	// Update player direction
	player->dir += angle;
	if (player->dir == 0)
		player->dir += 2 * PI;
	if (player->dir > 2 * PI)
		player->dir -= 2 * PI;

	// Rotate direction
	player->d->dx = player->d->dx * cos(angle) - player->d->dy * sin(angle);
	player->d->dy = oldDirX * sin(angle) + oldDirY * cos(angle);

	// Rotate plane
	player->plane->dx = player->plane->dx * cos(angle) - player->plane->dy * sin(angle);
	player->plane->dy = oldPlaneX * sin(angle) + oldPlaneY * cos(angle);
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
		check_collision(player, player->x - .05 * dy, player->y + .05 * dx, map);
	if (key == MLX_KEY_D)
		check_collision(player, player->x + .05 * dy, player->y - .05 * dx, map);
	if (key == MLX_KEY_W)
		check_collision(player, player->x + .1 * dx, player->y + .1 * dy, map);
	if (key == MLX_KEY_S)
		check_collision(player, player->x - .1 * dx, player->y - .1 * dy, map);
}
