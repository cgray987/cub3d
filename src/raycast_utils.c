/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:27:56 by cgray             #+#    #+#             */
/*   Updated: 2024/08/02 13:40:38 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* init t_ray values */
void	init_ray(t_ray *ray)
{
	ray->x = 0;
	ray->cam_x = 0;
	ray->ray_dir_x = 0;
	ray->ray_dir_y = 0;
	ray->delta_x = 0;
	ray->delta_y = 0;
	ray->side_dist_x = 0;
	ray->side_dist_y = 0;
	ray->perp_wall_dist = 0;
	ray->map_x = 0;
	ray->map_y = 0;
	ray->step_x = 0;
	ray->step_y = 0;
	ray->hit = 0;
	ray->side = 0;
	ray->line_height = 0;
	ray->draw_start = 0;
	ray->draw_end = 0;
}

/* sets distance for horizontal and vertical rays */
void	ray_direction(t_ray *ray, double x, double y)
{
	if (ray->ray_dir_x < 0.0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (x - ray->map_x) * ray->delta_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - x) * ray->delta_x;
	}
	if (ray->ray_dir_y < 0.0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (y - ray->map_y) * ray->delta_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - y) * ray->delta_y;
	}
}

/* sets perpendicular distance and calculates draw heights */
void	proj_distance(t_ray *ray, double *door_dist)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = (ray->side_dist_x - ray->delta_x);
		if (ray->door_hit == true)
			*door_dist = (ray->side_dist_x - ray->delta_x);
	}
	else
	{
		ray->perp_wall_dist = (ray->side_dist_y - ray->delta_y);
		if (ray->door_hit == true)
			*door_dist = (ray->side_dist_y - ray->delta_y);
	}
	ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2.0 + HEIGHT / 2.0;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2.0 + HEIGHT / 2.0;
	if (ray->draw_end >= HEIGHT)
		ray->draw_end = HEIGHT - 1;
}

/* gets ray direction from player, sets delta from direction
	delta -- len of ray from x or y to next x or y*/
void	ray_len(t_ray *ray, t_player *player)
{
	ray->ray_dir_x = player->d->dx + player->plane->dx * ray->cam_x;
	ray->ray_dir_y = player->d->dy + player->plane->dy * ray->cam_x;
	ray->map_x = (int)player->x;
	ray->map_y = (int)player->y;
	if (ray->ray_dir_x == 0.0)
		ray->delta_x = 1e30;
	else
		ray->delta_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0.0)
		ray->delta_y = 1e30;
	else
		ray->delta_y = fabs(1 / ray->ray_dir_y);
}
