/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 13:38:20 by cgray             #+#    #+#             */
/*   Updated: 2024/08/02 13:49:05 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*	side: 1 --> N/S
	side: 2 --> E/W
	0 = S
	1 = N
	2 = E
	3 = W*/
static int	get_side(int side, int step_x, int step_y)
{
	if (side == 1)
	{
		if (step_y > 0)
			return (0);
		else
			return (1);
	}
	else
	{
		if (step_x > 0)
			return (2);
		else
			return (3);
	}
}

static void	init_texture(t_txt_math *txt)
{
	txt->txt_index = 0;
	txt->wall_x = 0;
	txt->txt_x = 0;
	txt->txt_y = 0;
	txt->step = 0;
	txt->txt_pos = 0;
	txt->color = 0;
	txt->pixel_index = 0;
}

/* interpolates distance to wall and texture size
	norm makes this ugly as hell*/
static void	texture_math(t_game *game, t_ray *ray, t_txt_math *t)
{
	t->txt_width = game->map->texture_size;
	t->txt_index = get_side(ray->side, ray->step_x, ray->step_y);
	if (ray->side == 0)
		t->wall_x = game->player->y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		t->wall_x = game->player->x + ray->perp_wall_dist * ray->ray_dir_x;
	t->wall_x -= floor(t->wall_x);
	t->txt_x = (int)(t->wall_x * t->txt_width);
	if (ray->side == 0 && ray->ray_dir_x < 0)
		t->txt_x = t->txt_width - t->txt_x - 1;
	if (ray->side == 1 && ray->ray_dir_y > 0)
		t->txt_x = t->txt_width - t->txt_x - 1;
	t->step = 1.0 * t->txt_width / ray->line_height;
	t->txt_pos = (ray->draw_start - HEIGHT / 2.0
			+ ray->line_height / 2.0) * t->step;
}

/* draws vertical line using texture colors
	txt_y = cast txt coord to int and mask to prevent overflow*/
void	draw_texture(int x, int y, t_game *game, t_ray *ray)
{
	t_txt_math	*t;
	t_color		c;

	t = malloc(sizeof(t_txt_math));
	init_texture(t);
	texture_math(game, ray, t);
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		t->txt_y = (int) t->txt_pos & (t->txt_width - 1);
		t->txt_pos += t->step;
		t->pixel_index = (t->txt_width * t->txt_y + t->txt_x) * 4;
		c.r = game->texture[t->txt_index]->pixels[t->pixel_index];
		c.g = game->texture[t->txt_index]->pixels[t->pixel_index + 1];
		c.b = game->texture[t->txt_index]->pixels[t->pixel_index + 2];
		c.a = game->texture[t->txt_index]->pixels[t->pixel_index + 3];
		t->color = combine_color(c.r, c.g, c.b, c.a);
		if (ray->side == 0)
			t->color = fog(t->color, 5);
		t->color = fog(t->color, ray->perp_wall_dist);
		safe_pixel_put(game->img, x, y++, t->color);
	}
	free(t);
}
