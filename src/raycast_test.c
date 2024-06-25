/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:41:01 by cgray             #+#    #+#             */
/*   Updated: 2024/06/21 16:49:58 by cgray            ###   ########.fr       */
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
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1
	};
int		map_w = 8;
int		map_h = 8;
int		map_s = 64;

float	px = 100, py = 100;
float	pa = 90;
float	pdx = .5, pdy =.5;

float fix_angle(float a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}

float deg_to_rad(float deg)
{
	return deg * PI / 180;
}

void	safe_put_pixel(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && x < S_WIDTH && y >= 0 && y < S_HEIGHT)
		mlx_put_pixel(img, x, y, color);
}

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
		safe_put_pixel(img, p1.x, p1.y, color);
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

void	cast_rays(mlx_image_t *img)
{
	int r,mx,my,mp,dof;
	float rx,ry,ra,xo,yo;

	ra = fix_angle(pa);
	printf("ra: %.1f\n", ra);
	r = 0;
	while (r < 1)
	{
		dof = 0;
		float aTan = -1/tan(ra);
		if (sin(deg_to_rad(ra)) > .001) //looking up
		{
			ry = (((int)py>>6)<<6)-.0001;
			rx = (py-ry)*aTan+px;
			yo = -64;
			xo = -yo*aTan;
		}
		else if (sin(deg_to_rad(ra)) < .001)
		{
			ry = (((int)py>>6)<<6) + 64;
			rx = (py-ry)*aTan+px;
			yo = 64;
			xo = -yo*aTan;
		}
		else if (ra == 0 || ra == PI)
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		else
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx)>>6;
			my = (int) (ry)>>6;
			mp = my * map_w + mx;
			if (mp > 0 && mp < map_s && map[mp] == 1)
			{
				dof = 8;
				// ray hit a wall
			}
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}
		printf("rx: %.1f, ry: %.1f\n", rx, ry);
		printf("mx: %d, my: %d\n", mx, my);
		t_point v1, v2;
		v1.x = px;
		v1.y = py;
		v2.x = rx;
		v2.y = ry;
		draw_line(img, v1, v2, 0x0000ffff);
		r++;
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
	draw_line(img, l1, l2, 0x00ff00ff);
	cast_rays(img);
}

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	mlx_image_t	*img;
	// mlx_key_data_t	keydata;
}		t_keys;

void	key_hook(mlx_key_data_t keydata, void *v_keys)
{
	t_keys	*keys;
	int		pos;

	keys = (t_keys *)v_keys;

	int mx = (int)px>>6;
	int my = (int)py>>6;
	pos = my * map_w * mx;

	ft_memset(keys->img->pixels, 0, S_WIDTH * S_HEIGHT * 4);
	draw_map(keys->img);

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

	if (keys->a)
	{
		pa += 5; pa = fix_angle(pa); pdx = cos(pa * PI / 180); pdy = -sin(pa * PI / 180);
	}
	if (keys->d)
	{
		pa -= 5; pa = fix_angle(pa); pdx = cos(pa * PI / 180); pdy = -sin(pa * PI / 180);
	}
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
	if (px < 40)
		px = 40;
	if (px + 40 >= S_WIDTH)
		px = S_WIDTH - 40;
	if (py < 40)
		py = 40;
	if (py + 40 >= S_HEIGHT)
		py = S_HEIGHT - 40;


	if (keydata.key == MLX_KEY_W && keydata.action == MLX_RELEASE)
		keys->w = 0;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_RELEASE)
		keys->a = 0;
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_RELEASE)
		keys->s = 0;
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_RELEASE)
		keys->d = 0;

	// printf("w: %d, a: %d, s: %d, d: %d\n", keys->w, keys->a, keys->s, keys->d);
	// printf("px: %.1f, py: %.1f, pa: %.1f\n", px, py, pa);
	// printf("pdx: %.1f, pdy: %.1f\n", pdx, pdy);
	printf("map[%d] wall = %d\n",pos, map[pos]);
	draw_player(keys->img, px, py);
}

int	main(void)
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_keys		*keys;

	mlx = mlx_init(S_WIDTH, S_HEIGHT, "raycast", 1);
	img = mlx_new_image(mlx, S_WIDTH, S_HEIGHT);
	// keys = (t_keys *)malloc(sizeof(t_keys));
	keys->img = img;
	keys->w = 0;
	keys->a = 0;
	keys->s = 0;
	keys->d = 0;
	pdx = cos(pa * PI / 180);
	pdy = -sin(pa * PI / 180);
	// draw_map(img);
	// draw_player(img, px, py);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_key_hook(mlx, &key_hook, keys);
	// mlx_key_hook(mlx, &key_hook_release, keys);
	// mlx_key_hook(mlx, &button_down, keys);
	// mlx_key_hook(mlx, &button_up, keys);
	// key_hook(img, keys);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (0);
}
