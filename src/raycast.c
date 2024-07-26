/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 16:38:27 by cgray             #+#    #+#             */
/*   Updated: 2024/07/26 17:34:17 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


void	draw_v_line(int x, int start, int end, uint32_t color, t_game *game)
{
	int	y;

	y = 0;
	if (start >= HEIGHT || end < 0)
		return ;
	while (y < start) //draw floor
		safe_pixel_put(game->img, x, y++, game->ceiling); //0x9dbbd4ff
	while (y < end) //draw wall
		safe_pixel_put(game->img, x, y++, color);
	y = end;
	while (y < HEIGHT) //draw ceiling
		safe_pixel_put(game->img, x, y++, game->floor); //0xdbc6a7ff
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
		if (map[ray->map_y][ray->map_x] == '1')
		{
			ray->hit = true;
		}
	}
}

//https://lodev.org/cgtutor/raycasting.html#The_Basic_Idea_
void	raycast_start(t_game *game)
{
	int		x;
	t_ray	*ray;

	x = 0;
	game->perp_wall_dist = 0;
	ray = malloc(sizeof(t_ray));
	init_ray(ray);
	while (x++ < WIDTH)
	{
		ray->side = 0;
		ray->perp_wall_dist = 0;
		ray->cam_x = 2.0 * x / WIDTH - 1;
		ray_len(ray, game->player);
		ray_direction(ray, game->player->x, game->player->y);
		ray->hit = false;
		dda(ray, game->map->map);
		proj_distance(ray);
		if (x == WIDTH / 2)
			game->perp_wall_dist = ray->perp_wall_dist;
		int color1 = 0xe697e0ff;
		if (ray->side == 0)
			color1 = color1 * 2;
		color1 = fog(color1, ray->perp_wall_dist);
		draw_v_line(x, ray->draw_start, ray->draw_end, color1, game); //will need to change to add color
	}
}
