/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 17:25:47 by cgray             #+#    #+#             */
/*   Updated: 2024/07/26 17:26:15 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	combine_color(int r, int g, int b)
{
	return (r << 24 | g << 16 | b << 8 | 0xff);
}

void	color_limits(int *color_component)
{
	if (*color_component > 255)
		*color_component = 255;
	else if (*color_component < 0)
		*color_component = 0;
}

int	fog(int color, double wall_dist)
{
	double	darkness;
	int		r;
	int		g;
	int		b;

	darkness = 1 / (1 + wall_dist * .1);

	b = (color >> 8) & 0xFF;
	g = (color >> 16) & 0xFF;
	r = (color >> 24) & 0xFF;
	b = (int)(b * darkness);
	g = (int)(g * darkness);
	r = (int)(r * darkness);
	color_limits(&b);
	color_limits(&g);
	color_limits(&r);
	return (combine_color(r, g, b));
}
