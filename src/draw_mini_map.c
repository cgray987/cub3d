/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_mini_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:10:06 by cgray             #+#    #+#             */
/*   Updated: 2024/08/05 13:40:12 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_square(mlx_image_t *img, t_point p, int size, uint32_t color)
{
	t_point	end;

	end.x = p.x + size;
	end.y = p.y + size;
	while (p.y < end.y)
	{
		while (p.x < end.x)
		{
			safe_pixel_put(img, p.x, p.y, color);
			p.x++;
		}
		p.x -= size;
		p.y++;
	}
}

int	color_chooser(char **map, int x, int y)
{
	int	color;

	if (map[y][x] == '1')
		color = 0x7942d6ff;
	else if (map[y][x] == '0')
		color = 0xffffffff;
	else if (map[y][x] == 'D')
		color = 0xd16bccff;
	else
		color = 0xddddddff;
	return (color);
}

void	draw_minimap(t_game *game)
{
	int	x;
	int	y;
	int	color;
	int	ms;

	ms = game->mini_scale;
	y = 0;
	while (game->map->map[y])
	{
		x = 0;
		while (game->map->map[y][x])
		{
			if (game->map->map[y][x] == ' ')
			{
				x++;
				continue ;
			}
			color = color_chooser(game->map->map, x, y);
			draw_square(game->img, (t_point){x * ms,
				y * ms}, ms, color);
			x++;
		}
		y++;
	}
}

/* norm.... */
t_point	get_step(t_point start, t_point end)
{
	t_point	s;

	s.x = 1;
	s.y = 1;
	if (start.x > end.x)
		s.x = -1;
	if (start.y > end.y)
		s.y = -1;
	return (s);
}

/* bresenham, adapted from https://gist.github.com/bert/1085538*/
void	draw_line(t_point start, t_point end, uint32_t color, mlx_image_t *img)
{
	t_point	d;
	t_point	s;
	t_point	err;

	d.x = abs(end.x - start.x);
	d.y = -abs(end.y - start.y);
	err.x = d.x + d.y;
	s = get_step(start, end);
	while (1)
	{
		safe_pixel_put(img, start.x, start.y, color);
		if (start.x == end.x && start.y == end.y)
			break ;
		err.y = 2 * err.x;
		if (err.y >= d.y)
		{
			err.x += d.y;
			start.x += s.x;
		}
		if (err.y <= d.x)
		{
			err.x += d.x;
			start.y += s.y;
		}
	}
}
