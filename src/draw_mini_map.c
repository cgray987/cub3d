/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_mini_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:10:06 by cgray             #+#    #+#             */
/*   Updated: 2024/07/24 15:54:15 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clear_img(mlx_image_t *img)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
			mlx_put_pixel(img, x++, y, 0);
		y++;
	}
	//errors?
}

void	safe_pixel_put(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x < WIDTH && y < HEIGHT
		&& x > 0 && y > 0)
		mlx_put_pixel(img, x, y, color);
	//errors?
}

void	draw_square(mlx_image_t *img, int x, int y, int size, uint32_t color)
{
	int	x_end;
	int	y_end;

	x_end = x + size;
	y_end = y + size;
	while (y < y_end)
	{
		while (x < x_end)
		{
			safe_pixel_put(img, x, y, color);
			x++;
		}
		x -= size;
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	int			x;
	int			y;
	uint32_t	color;

	y = 0;
	while (game->map->map[y])
	{
		x = 0;
		while (game->map->map[y][x])
		{
			if (game->map->map[y][x] == '1')
				color = 0x7942d6ff;
			else if (game->map->map[y][x] == '0')
				color = 0xffffffff;
			else
				color = 0xddddddff;
			draw_square(game->img, x * MINI_SCALE, y * MINI_SCALE, MINI_SCALE, color);
			x++;
		}
		y++;
	}
}

/* bresenham, adapted from https://gist.github.com/bert/1085538*/
void	draw_line(int x0, int y0, int x1, int y1, uint32_t color, mlx_image_t *img)
{
	int	dx;
	int	dy;
	int sx;
	int	sy;
	int err;
	int	e2; /* error value e_xy */

	dx = abs(x1 - x0);
	dy = -abs(y1 - y0);
	err = dx + dy;
	sx = 1;
	sy = 1;
	if (x0 > x1)
		sx = -1;
	if (y0 > y1)
		sy = -1;
	while (1)
	{
		safe_pixel_put(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)/* e_xy+e_x > 0 */
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)/* e_xy+e_y < 0 */
		{
			err += dx;
			y0 += sy;
		}
	}
}
