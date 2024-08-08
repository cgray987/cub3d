/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:25:47 by cgray             #+#    #+#             */
/*   Updated: 2024/08/08 17:01:15 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* combines rgb color components into one int, full opacity */
int	combine_color(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

/* individual color component limits */
void	color_limits(int *color_component)
{
	if (*color_component > 255)
		*color_component = 255;
	else if (*color_component < 0)
		*color_component = 0;
}

/* darkens color based on distance */
int	fog(int color, double wall_dist)
{
	double	darkness;
	t_color	c;

	darkness = 1 / (1 + wall_dist * FOG);
	c.b = (color >> 8) & 0xFF;
	c.g = (color >> 16) & 0xFF;
	c.r = (color >> 24) & 0xFF;
	c.a = color & 0xFF;
	c.b = (int)(c.b * darkness);
	c.g = (int)(c.g * darkness);
	c.r = (int)(c.r * darkness);
	color_limits(&c.b);
	color_limits(&c.g);
	color_limits(&c.r);
	return (combine_color(c.r, c.g, c.b, c.a));
}

/* return color in between color1 and color2 based on height(y) */
int	grad_color(int color1, int color2, int y)
{
	float	factor;
	t_color	c1;
	t_color	c2;
	t_color	c_new;

	factor = 1.0 - (float)abs(y - (HEIGHT / 2)) / (HEIGHT / 2);
	c1.r = (color1 >> 24) & 0xFF;
	c1.g = (color1 >> 16) & 0xFF;
	c1.b = (color1 >> 8) & 0xFF;
	c2.r = (color2 >> 24) & 0xFF;
	c2.g = (color2 >> 16) & 0xFF;
	c2.b = (color2 >> 8) & 0xFF;
	c_new.r = c1.r + factor * (c2.r - c1.r);
	c_new.g = c1.g + factor * (c2.g - c1.g);
	c_new.b = c1.b + factor * (c2.b - c1.b);
	return (combine_color(c_new.r, c_new.g, c_new.b, 0xff));
}
