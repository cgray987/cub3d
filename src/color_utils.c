/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:25:47 by cgray             #+#    #+#             */
/*   Updated: 2024/08/02 12:45:33 by cgray            ###   ########.fr       */
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
