/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:41:01 by cgray             #+#    #+#             */
/*   Updated: 2024/06/05 16:45:41 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define S_WIDTH 800
#define S_HEIGHT 600
#define PI 3.14159265359

typedef struct s_point
{
	int	x;
	int	y;
}		t_point;

int		map[] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
		0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};
int		map_w = 8;
int		map_h = 8;
int		map_s = 64;

float	px = 100, py = 100;
float	pa = 90;
float	pdx = 0.5, pdy = 0.5;

void	draw_line(mlx_image_t *img, t_point p1, t_point p2, uint32_t color)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(p2.x - p1.x);
	dy = abs(p2.y - p1.y);
	sx = (p1.x < p2.x) ? 1 : -1;
	sy = (p1.y < p2.y) ? 1 : -1;
	err = (dx > dy ? dx : -dy) / 2;
	while (p1.x != p2.x || p1.y != p2.y)
	{
		mlx_put_pixel(img, p1.x, p1.y, color);
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			p1.x += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			p1.y += sy;
		}
	}
}

void	draw_square(mlx_image_t *img, t_point top_left, t_point bottom_right,
		uint32_t color)
{
	int x, y;
	y = top_left.y;
	while (y++ < bottom_right.y)
	{
		x = top_left.x;
		while (x++ < bottom_right.x)
		{
			mlx_put_pixel(img, x, y, color);
		}
	}
}

void	draw_map(mlx_image_t *img)
{
	int	color;

	int x, y, xoff, yoff;
	t_point v1, v2;
	x = 0, y = 0;
	while (y < map_h)
	{
		x = 0;
		while (x < map_w)
		{
			if (map[y * map_w + x] == 1)
				color = 0xffffffff;
			else
				color = 0x000000ff;
			xoff = x * map_s;
			yoff = y * map_s;
			v1.x = 0 + xoff + 1;
			v1.y = 0 + yoff + 1;
			v2.y = map_s + yoff - 1;
			v2.x = map_s + xoff - 1;
			draw_square(img, v1, v2, color);
			x++;
		}
		y++;
	}
}

void	draw_player(mlx_image_t *img, int px, int py)
{
	t_point v1, v2, l1, l2;
	v1.x = px;
	v1.y = py;
	v2.x = px + 10;
	v2.y = py + 10;
	l1.x = px;
	l1.y = py;
	l2.x = px + pdx * 40;
	l2.y = py + pdy * 40;
	draw_square(img, v1, v2, 0xff0000ff);
	// draw line for player direction
	draw_line(img, l1, l2, 0xee0000ff);
}

int	fix_angle(int a)
{
	if (a > 359)
		a = -359;
	if (a < 0)
		a = 360;
	return (a);
}

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
}		t_keys;

void	button_down(mlx_key_data_t keydata, void *v_keys)
{
	t_keys	*keys;

	keys = (t_keys *)v_keys;
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
		keys->w = 1;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		keys->a = 1;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
		keys->s = 1;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		keys->d = 1;

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		exit(0);

}

void	button_up(mlx_key_data_t keydata, void *v_keys)
{
	t_keys	*keys;

	keys = (t_keys *)v_keys;
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_RELEASE)
		keys->w = 0;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE)
		keys->a = 0;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_RELEASE)
		keys->s = 0;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE)
		keys->d = 0;

	key_hook(img, keys);
}

void	key_hook(void *img, t_keys *keys)
{
	img = (mlx_image_t *)img;
	draw_map(img);


	if (keys->a)
		pa -= 5; pa = fix_angle(pa); pdx = cos(pa * PI / 180); pdy = -sin(pa * PI / 180);
	if (keys->d)
		pa += 5; pa = fix_angle(pa); pdx = cos(pa * PI / 180); pdy = -sin(pa * PI / 180);
	if (keys->w)
	{
		px += pdx * 5;
		py += pdy * 5;
	}
	if (keys->s)
	{
		px -= pdx * 5;
		py -= pdy * 5;
	}

	if (px < 0)
		px = 0;
	if (px > S_WIDTH)
		px = S_WIDTH;
	if (py < 0)
		py = 0;
	if (py > S_HEIGHT)
		py = S_HEIGHT;
	if (pa < 0)
		pa = 360;
	if (pa > 360)
		pa = 0;
	// printf("px: %d, py: %d, pa: %d\n", px, py, pa);
	// printf("pdx: %f, pdy: %f\n", pdx, pdy);
	draw_player(img, px, py);
}

int	main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_keys		*keys;

	mlx = mlx_init(S_WIDTH, S_HEIGHT, "raycast", 1);
	img = mlx_new_image(mlx, S_WIDTH, S_HEIGHT);
	pdx = cos(pa * PI / 180);
	pdy = -sin(pa * PI / 180);
	draw_map(img);
	draw_player(img, px, py);
	mlx_image_to_window(mlx, img, 0, 0);
	// mlx_key_hook(mlx, &key_hook, img);
	mlx_key_hook(mlx, &button_down, keys);
	mlx_key_hook(mlx, &button_up, keys);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
