/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgray <cgray@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 15:03:29 by cgray             #+#    #+#             */
/*   Updated: 2024/03/28 15:04:22 by cgray            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* returns 1 if character c is space or whitespace */
int	ft_isspace(int c)
{
	char *spaces = " \t\n\v\f\r";

	while (*spaces)
	{
		if (c == *spaces)
			return (1);
		spaces++;
	}
	return (0);
}
