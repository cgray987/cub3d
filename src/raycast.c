/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:38:27 by cgray             #+#    #+#             */
/*   Updated: 2024/08/08 16:56:50 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* draws vertical line at given x position
	draw ceiling until ray start
		gradient between ceiling and floor color
	draw texture until ray end
	draw floor until screen height*/
void	draw_v_line(int x, t_game *game, t_ray *ray)
{
	int	y;
	int	color;

	y = 0;
	if (ray->draw_start >= HEIGHT || ray->draw_end < 0)
		return ;
	color = game->ceiling;
	while (y < ray->draw_start)
	{
		safe_pixel_put(game->img, x, y,
			grad_color(game->ceiling, game->floor, y));
		y++;
	}
	draw_texture(x, y, game, ray);
	y = ray->draw_end;
	color = game->floor;
	while (y < HEIGHT)
		safe_pixel_put(game->img, x, y++,
			game->floor);
}

/* adds to side distances until a wall is reached in map */
void	dda(t_ray *ray, char **map)
{
	while (ray->hit == false)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (map[ray->map_y][ray->map_x] == 'D'
			|| map[ray->map_y][ray->map_x] == 'O')
			ray->door_hit = true;
		if (map[ray->map_y][ray->map_x] == '1'
			|| map[ray->map_y][ray->map_x] == 'D')
			ray->hit = true;
	}
}

//https://lodev.org/cgtutor/raycasting.html#The_Basic_Idea_
void	raycast_start(t_game *game)
{
	int		x;
	t_ray	*ray;

	x = 0;
	game->perp_wall_dist = 0;
	game->door_dist = 0;
	ray = malloc(sizeof(t_ray));
	init_ray(ray);
	while (x++ < WIDTH)
	{
		ray->x = x;
		ray->side = 0;
		ray->perp_wall_dist = 0;
		ray->cam_x = 2.0 * x / WIDTH - 1;
		ray_len(ray, game->player);
		ray_direction(ray, game->player->x, game->player->y);
		ray->hit = false;
		ray->door_hit = false;
		dda(ray, game->map->map);
		proj_distance(ray, &game->door_dist);
		if (x == WIDTH / 2)
			game->perp_wall_dist = ray->perp_wall_dist;
		draw_v_line(x, game, ray);
	}
	free(ray);
}
