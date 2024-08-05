/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 14:06:26 by cgray             #+#    #+#             */
/*   Updated: 2024/08/01 14:10:28 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* iterates thru image clearing previous img
	(unsure if this will be needed) */
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
}

/* places pixels if they're inbound */
void	safe_pixel_put(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x < WIDTH && y < HEIGHT
		&& x > 0 && y > 0)
		mlx_put_pixel(img, x, y, color);
}
