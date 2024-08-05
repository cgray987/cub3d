/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:50:51 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 13:51:35 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* checks if player will hit wall with new position */
void	check_collision(t_player *player, double new_x, double new_y,
		char **map)
{
	if (map[(int)new_y][(int)player->x] != '1'
		&& map[(int)new_y][(int)player->x] != 'D')
		player->y = new_y;
	if (map[(int)player->y][(int)new_x] != '1'
		&& map[(int)player->y][(int)new_x] != 'D')
		player->x = new_x;
}

/* Sets door to 'O'(open) if 'D'(closed)
	Sets door to 'D'(closed) if 'O'(open)
	if you are not inside of door */
void	door_handler(t_game *game, int i, int j)
{
	if (game->map->map[i][j] == 'D')
		game->map->map[i][j] = 'O';
	else if (game->map->map[i][j] == 'O'
		&& game->door_dist > 0.1
		&& (i != (int)game->player->y || j != (int)game->player->x))
		game->map->map[i][j] = 'D';
}
